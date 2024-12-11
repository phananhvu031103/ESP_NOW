set(app_srcs "led_strip.c")
set(pri_req driver)
idf_component_register(SRCS ${app_srcs}
        INCLUDE_DIRS "."
        REQUIRES ${pri_req})

                     