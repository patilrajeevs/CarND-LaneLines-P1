cmake_minimum_required(VERSION 3.5)

project(lane_lines)

add_executable(lane_lines src/lane_lines.cpp)

find_package( OpenCV REQUIRED )
target_link_libraries(lane_lines ${OpenCV_LIBS} )
target_link_libraries(lane_lines opencv_helper )
target_link_libraries(lane_lines geometry_helper )

set_target_properties(lane_lines
            PROPERTIES CXX_STANDARD 11
           )


#find_package(opencv_helper REQUIRED)
#find_package(geometry_helper REQUIRED)


