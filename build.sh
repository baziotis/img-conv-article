gcc 2d_conv_ser.cpp -o ./bin/2d_conv_ser -O2
gcc 1d_conv_ser.cpp -o ./bin/1d_conv_ser -O2
gcc 1d_conv_vec.cpp -o ./bin/1d_conv_vec -O2 -march=native
gcc 2d_conv_vec.cpp -o ./bin/2d_conv_vec -O2 -march=native
gcc 2d_conv_vec_rgb.cpp -o ./bin/2d_conv_vec_rgb -O2 -march=native