#include "logger.h"
#include "logger_builder_concrete.h"

int main()
{
    logger_builder *builder = new logger_builder_concrete();
    logger_builder *builder2 = new logger_builder_concrete();
    logger *cp = builder2->add_stream("bbb.txt", logger::severity::warning)->construct();
    logger *constructed_logger = builder
                                     ->add_stream("file1.txt", logger::severity::critical)
                                     ->add_stream("file2.txt", logger::severity::debug)
                                     ->add_stream("file3.txt", logger::severity::trace)
                                     ->construct();

    logger *constructed_logger_2 = builder
                                       ->add_stream("file4.txt", logger::severity::warning)
                                       ->construct();

    constructed_logger
        ->log("kek lol 123", logger::severity::critical)
        ->log("puppuk", logger::severity::critical);

    constructed_logger_2
        ->log("123 kek lol", logger::severity::error);
    cp->log("kwakwa", logger::severity::error);
    delete constructed_logger_2;
    delete constructed_logger;
    delete builder;

    return 0;
}
