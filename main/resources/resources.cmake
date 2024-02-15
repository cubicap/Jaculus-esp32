file(READ ${resources_dir}/ts_examples.tar.gz TS_EXAMPLES_TGZ HEX)

configure_file(${src_in} ${src_out} @ONLY)
