set(app_src dht11.c)
set(pri_req driver)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req}
                    PRIV_REQUIRES esp_timer)

