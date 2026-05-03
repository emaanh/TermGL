#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include <thread>

int main() {
    //char ascii[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.";
    //char ascii[] = ".,:ilwW";
    //char ascii[] = "█▓▒░";
    char ascii[] = "#@%*=+:-. ";
    //char ascii[] = "@#W$9876543210?!abc;:+=-,._";
    int num_ascii = strlen(ascii);
    bool fps_init = false; 
    float spf_avg = 0.0f; 
    int constexpr target_fps = 60; 
   // float constexpr target_spf = 1.0f / target_fps;
    float constexpr alpha = 1.0f/(target_fps+1);
    constexpr auto target_duration = std::chrono::microseconds(1000000 / target_fps);
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int screen_margin = 2;
    int rows = w.ws_row-screen_margin;
    int cols = w.ws_col-screen_margin;
    int buffer_size = (cols+1) * rows;
    char* z_buffer = new char[buffer_size];
    printf("\033[?25l"); //hide cursor  
    int count = 0;
    auto start = std::chrono::steady_clock::now();

    for(;;) {
        for(int y = 0 ; y < rows; y++) {
            for(int x = 0; x < cols; x++) {
                int pixel_idx = y*(cols+1)+x;
                float comp = ((((rows/2.0f-y)*(rows/2.0f -y) + (cols/2.0f-x)*(cols/2.0f-x)*0.25))/(rows/2.0f * rows/2.0f + cols/2.0f * cols/2.0f* 0.25));
                char pixel = pixel_idx < count ? ' ' : ascii[std::min((int)(comp*3*num_ascii),num_ascii-1)];
                z_buffer[pixel_idx] = pixel;
            }
            z_buffer[y*(cols+1) + cols] = '\n';
        }
        z_buffer[buffer_size-1] = '\0';
        if(count >= buffer_size) count = 0;
        count ++;
 
        printf("\033[H");
        fwrite(z_buffer, 1, buffer_size, stdout);
        fflush(stdout);
        auto target = start + target_duration;
        std::this_thread::sleep_until(target-std::chrono::milliseconds(4));
            
        while(std::chrono::steady_clock::now() < target) __asm__ volatile("yield"); 
        auto stop = std::chrono::steady_clock::now();
        
        float spf_measured = std::chrono::duration<float>(stop - start).count(); //seconds
        start = target;
        
        float fps = 1.0f/spf_measured;
//      printf("\n FPS: : %.3f,", fps);

    }
    delete[] z_buffer;
    printf("\033[?25h"); //show cursor
}
