#include "list_memory.h"
#include <cstring>

void *list_memory::get_pointer_block(void *target_ptr) const
{
    return *reinterpret_cast<void **>(reinterpret_cast<size_t *>(target_ptr) + 1);
}

size_t list_memory::get_size_block(void *target_ptr) const
{
    return *reinterpret_cast<size_t *>(target_ptr);
}

void *list_memory::get_first_avail_block() const
{
    return *get_first_avail_block_ptr();
}

void **list_memory::get_first_avail_block_ptr() const
{
    return reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size() - sizeof(void *));
}

size_t list_memory::get_service_part_allocator_size() const
{
    return sizeof(size_t) + sizeof(logger *) + sizeof(allocate_mode) + sizeof(memory *) + sizeof(void *);
}

size_t list_memory::get_service_part_avail_block_size() const
{
    return sizeof(size_t) + sizeof(void *);
}

logger *list_memory::get_logger() const noexcept
{
    return *reinterpret_cast<logger **>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + sizeof(size_t));
}

void *list_memory::get_right_border_of_memory_ptr() const
{
    return reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size() + *reinterpret_cast<size_t *>(_ptr_to_memory));
}

std::string list_memory::get_allocate_mode_string(allocate_mode method) const
{
    std::string allocate_mode;
    switch (method)
    {
    case memory::allocate_mode::first_fit:
        allocate_mode = "the first fit method";
        break;

    case memory::allocate_mode::best_fit:
        allocate_mode = "the best fit method";
        break;
    default:
        allocate_mode = "the worst fit method";
        break;
    }
    return allocate_mode;
}

void *list_memory::allocate(size_t target_size) const
{
    auto overridden_block_size = target_size;

    if (overridden_block_size < sizeof(void *))
    {
        overridden_block_size = sizeof(void *);
    }

    auto alloc_method = *reinterpret_cast<allocate_mode *>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + sizeof(size_t) + sizeof(logger *));

    void *prev_block = nullptr;
    void *curr_block = get_first_avail_block();

    void *prev_to_target_block = nullptr;
    void *target_block = nullptr;
    void *next_to_target_block = nullptr;

    size_t curr_size_block;

    while (curr_block != nullptr)
    {
        void *next_block = get_pointer_block(curr_block);
        curr_size_block = get_size_block(curr_block);

        if (curr_size_block + sizeof(void *) >= overridden_block_size)
        {
            if (alloc_method == memory::allocate_mode::first_fit ||
                alloc_method == memory::allocate_mode::best_fit && (target_block == nullptr || get_size_block(target_block) > curr_size_block) ||
                alloc_method == memory::allocate_mode::worst_fit && (target_block == nullptr || get_size_block(target_block) < curr_size_block))
            {
                prev_to_target_block = prev_block;
                target_block = curr_block;
                next_to_target_block = next_block;
            }

            if (alloc_method == memory::allocate_mode::first_fit)
            {
                break;
            }
        }

        prev_block = curr_block;
        curr_block = next_block;
    }

    if (target_block == nullptr)
    {
        throw std::bad_alloc();
    }

    auto avail_block_serv_part_size = get_service_part_avail_block_size();
    auto occup_block_serv_part_size = sizeof(size_t);
    size_t target_block_size = get_size_block(target_block);

    if (target_block_size + sizeof(void *) - overridden_block_size < avail_block_serv_part_size)
    {
        overridden_block_size = target_block_size + sizeof(void *);
    }

    if (overridden_block_size != target_size)
    {
        this->debug_with_guard("Size has changed: " + std::to_string(target_size) + "->" + std::to_string(overridden_block_size));
        target_size = overridden_block_size;
    }

    void *next_for_prev;

    if (target_block_size + avail_block_serv_part_size == target_size + occup_block_serv_part_size)
    {
        next_for_prev = next_to_target_block;
    }
    else
    {
        next_for_prev = reinterpret_cast<void *>(reinterpret_cast<unsigned char *>(target_block) + target_size + sizeof(size_t));
        *reinterpret_cast<size_t *>(next_for_prev) = target_block_size - target_size - sizeof(size_t);
        *reinterpret_cast<void **>(reinterpret_cast<size_t *>(next_for_prev) + 1) = next_to_target_block;
    }

    if (prev_to_target_block == nullptr)
    {
        *get_first_avail_block_ptr() = next_for_prev;
    }
    else
    {
        *reinterpret_cast<void **>(reinterpret_cast<size_t *>(prev_to_target_block) + 1) = next_for_prev;
    }

    *reinterpret_cast<size_t *>(target_block) = target_size;

    this->trace_with_guard("Allocate is " + get_allocate_mode_string(alloc_method));
    dump_allocator_state(true);

    return reinterpret_cast<void *>(reinterpret_cast<size_t *>(target_block) + 1);
}

void list_memory::deallocate(void *block_to_deallocate_address) const
{

    auto true_block_ptr = reinterpret_cast<void *>(reinterpret_cast<size_t *>(block_to_deallocate_address) - 1);
    debug_alloc(true_block_ptr);
    auto curr_block = get_first_avail_block();

    if ((true_block_ptr < reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size()) || (true_block_ptr >= reinterpret_cast<unsigned char *>(get_right_border_of_memory_ptr()) - sizeof(size_t *) - sizeof(void *)))
    {
        throw list_memory::dealloc_fail();
    }

    void *prev_block = nullptr;

    while (curr_block < block_to_deallocate_address)
    {
        prev_block = curr_block;
        curr_block = get_pointer_block(curr_block);
    }

    auto block_to_deallocate_size = *reinterpret_cast<size_t *>(true_block_ptr);
    if (prev_block == nullptr && curr_block == nullptr)
    {
        *reinterpret_cast<void **>(const_cast<void *>(block_to_deallocate_address)) = nullptr;
        *get_first_avail_block_ptr() = true_block_ptr;
    }
    else
    {
        if (curr_block == nullptr)
        {
            *reinterpret_cast<void **>(const_cast<void *>(block_to_deallocate_address)) = nullptr;
            block_to_deallocate_size = *reinterpret_cast<size_t *>(true_block_ptr) = block_to_deallocate_size - sizeof(void *);
        }
        else
        {
            if (reinterpret_cast<unsigned char *>(const_cast<void *>(block_to_deallocate_address)) + block_to_deallocate_size == curr_block)
            {
                *reinterpret_cast<void **>(reinterpret_cast<size_t *>(true_block_ptr) + 1) = get_pointer_block(curr_block);
                block_to_deallocate_size = *reinterpret_cast<size_t *>(true_block_ptr) = sizeof(size_t) + block_to_deallocate_size + get_size_block(curr_block);
                curr_block = true_block_ptr;
            }
            else
            {
                *reinterpret_cast<void **>(reinterpret_cast<size_t *>(true_block_ptr) + 1) = curr_block;
                block_to_deallocate_size = *reinterpret_cast<size_t *>(true_block_ptr) = block_to_deallocate_size - sizeof(void *);
            }
        }

        if (prev_block == nullptr)
        {
            *get_first_avail_block_ptr() = true_block_ptr;
        }
        else
        {
            auto prev_block_size = get_size_block(prev_block);
            if (reinterpret_cast<unsigned char *>(prev_block) + get_service_part_avail_block_size() + prev_block_size == true_block_ptr)
            {
                *reinterpret_cast<void **>(reinterpret_cast<size_t *>(prev_block) + 1) = get_pointer_block(true_block_ptr);
                *reinterpret_cast<size_t *>(prev_block) = prev_block_size + block_to_deallocate_size + get_service_part_avail_block_size();
            }
            else
            {
                *reinterpret_cast<void **>(reinterpret_cast<size_t *>(prev_block) + 1) = true_block_ptr;
            }
        }
    }
    dump_allocator_state(false);
}

std::string list_memory::get_pointer_address(void *target_ptr) const
{

    char str[20];
    memset(str, 0, 20);
    sprintf(str, "%p", target_ptr);
    std::string add;
    add.append(str);
    return add;
}

void list_memory::debug_alloc(const void *target_ptr) const
{
    logger *log = get_logger();
    if (log == nullptr)
    {
        return;
    }
    unsigned char *ptr = reinterpret_cast<unsigned char *>(reinterpret_cast<size_t *>(const_cast<void *>(target_ptr)) + 1);
    size_t size = get_size_block(const_cast<void *>(target_ptr));
    std::string buff;
    for (int i = 0; i < size; i++)
    {
        unsigned short tmp = static_cast<unsigned short>(*ptr);
        buff.append(std::to_string(tmp) + ' ');
        ptr++;
    }
    std::string add = get_pointer_address(const_cast<void *>(target_ptr));
    std::string massege = "Block at address " + add + " state before deallocation:\n[ " + buff + "]";
    log->log(massege, logger::severity::debug);
}

list_memory::list_memory(logger *log, memory *allocator, size_t size, allocate_mode method)
{

    auto target_size = size + get_service_part_allocator_size();
    _ptr_to_memory = reinterpret_cast<void *>(allocator != nullptr ? allocator->allocate(target_size) : ::operator new(target_size));

    auto total_size = reinterpret_cast<size_t *>(_ptr_to_memory);
    *total_size = size;
    auto log_block = reinterpret_cast<logger **>(total_size + 1);
    *log_block = log;
    auto mode_block = reinterpret_cast<allocate_mode *>(log_block + 1);
    *mode_block = method;
    auto alloc_block = reinterpret_cast<memory **>(mode_block + 1);
    *alloc_block = allocator;
    auto first_avail_block_block = reinterpret_cast<void **>(alloc_block + 1);
    *first_avail_block_block = first_avail_block_block + 1;
    auto *tmp = reinterpret_cast<size_t *>(*first_avail_block_block);
    *tmp = size - sizeof(void *) - sizeof(size_t);
    *reinterpret_cast<void **>(tmp + 1) = nullptr;

    this->trace_with_guard("List system allocator initialized!");
}

list_memory::~list_memory()
{
    if (_ptr_to_memory == nullptr)
        return;

    auto *got_logger = get_logger();

    auto allocator_block = *reinterpret_cast<memory **>(reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size() - sizeof(void *) - sizeof(memory *));

    allocator_block == nullptr ? ::operator delete(_ptr_to_memory) : allocator_block->deallocate(_ptr_to_memory);

    if (got_logger != nullptr)
    {
        got_logger->log("Destructor executed successfully", logger::severity::trace);
    }
}

void list_memory::dump_allocator_state(bool is_allocate) const noexcept
{
    auto *log = get_logger();
    if (log == nullptr)
    {

        return;
    }

    auto allocator_memory_size = *reinterpret_cast<size_t *>(_ptr_to_memory);

    std::string constructed_state_string("|");
    auto *current_aviable_block = get_first_avail_block();
    auto *current_any_block = reinterpret_cast<unsigned char *>(_ptr_to_memory) + get_service_part_allocator_size();

    while (current_any_block - reinterpret_cast<unsigned char *>(_ptr_to_memory) < allocator_memory_size)
    {
        size_t any_block_size = 0;
        if (current_any_block == current_aviable_block)
        {
            constructed_state_string += "avlbl " + std::to_string(any_block_size = get_size_block(current_any_block)) + '|';
            any_block_size += get_service_part_avail_block_size();
            current_aviable_block = get_pointer_block(current_aviable_block);
        }
        else
        {
            constructed_state_string += "occpd " + std::to_string(any_block_size = get_size_block(current_any_block)) + '|';
            any_block_size += sizeof(size_t);
        }

        current_any_block += any_block_size;
    }

    log->log(std::string("Memory state after ") + (is_allocate ? "" : "de") + "allocation operation: " + constructed_state_string, logger::severity::trace);
}
