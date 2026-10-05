target_sources(app PRIVATE 
               main.cpp
            #    drivers/our_driver/our_driver.c
                #  drivers/our_driver/our_driver.h
               )

target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/drivers/our_driver)