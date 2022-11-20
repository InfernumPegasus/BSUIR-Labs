#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <filesystem>
#include <linux/input.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define PHOTO_PATH "./photo/"
#define VIDEO_PATH "./video/"

bool recording = false;

void clear_console() {
    printf("\033[2J\033[1;1H");
}

void print_webcams_info() {
    for (auto &p : std::filesystem::directory_iterator("/sys/class/video4linux")) {
        std::string filename = p.path().string();
        printf("Device folder: '%s'\n", filename.c_str());

        std::ifstream ifs;
        std::string line;

        // getting device name
        ifs.open(filename + "/name");
        std::getline(ifs, line);
        printf("Device name: %s\n", line.c_str());
        ifs.close();

        // getting driver info
        ifs.open(filename + "/device/driver/1-3:1.0/uevent");
        while (std::getline(ifs, line)) {
            printf("%s\n", line.c_str());
        }
        ifs.close();
        printf("\n");
    }
}

std::string get_current_date_time() {
    time_t     now = time(nullptr);
    struct tm  time_struct = *localtime(&now);
    char       buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &time_struct);

    return std::string { buf };
}

void save_frame(const cv::Mat& frame) {
    const std::string image_name = "image";
    const auto full_save_name = PHOTO_PATH + image_name + "-" + get_current_date_time() + ".jpg";

    cv::imwrite(full_save_name, frame);
}

std::string get_kb_device() {
    std::string kb_device;
    /*
     * Iterate over all files in '/dev/input' and read status.
     * If file's status is character file's status it can be keyboard events' file.
     */
    for (auto &p : std::filesystem::directory_iterator("/dev/input/")) {
        std::filesystem::file_status status = std::filesystem::status(p);
        if (std::filesystem::is_character_file(status)) {
            // full path to event file
            std::string filename = p.path().string();

            // opening event file in read-only mode
            int fd = open(filename.c_str(), O_RDONLY);
            if (fd == -1) {
                std::cerr << "Error: " << strerror(errno) << std::endl;
                continue;
            }

            int32_t event_bitmap = 0;
            /*
             * Keyboard keys codes of A, B, C, Z
             * These codes are needed to check whether this
             * event file is a keyboard event file
             */
            int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z;

            // Check if keyboard events supported
            ioctl(fd, EVIOCGBIT(0, sizeof(event_bitmap)), &event_bitmap);
            if ((EV_KEY & event_bitmap) == EV_KEY) {
                // Check if A, B, C, Z keys are supported by keyboard
                ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);
                if ((kbd_bitmap & event_bitmap) == kbd_bitmap) {
                    kb_device = filename;
                    close(fd);
                    break;
                }
            }
            close(fd);
        }
    }
    return kb_device;
}

void hidden_camera_menu() {
    auto pid = fork();
    if (pid == -1) {
        std::cerr << "Fork error\n";
        exit(1);
    } else if (pid != 0) {
        std::cerr << "Parent process\n";
        exit(0);
    } else {
        // Replacing file descriptors for stdin and stderr
        int log_fd = open("log.txt", O_WRONLY);
        if (log_fd == -1) {
            std::cerr << "open error\n";
            exit(1);
        }
        dup2(log_fd, STDIN_FILENO);
        dup2(log_fd, STDERR_FILENO);

        std::cerr << "Child process\n";

        // Keylogger initialization.
        auto device = get_kb_device();
        int fd = open(device.c_str(), O_RDONLY);
        struct input_event ev{};

        // Instantiating default camera capture
        cv::VideoCapture videoCapture(0);
        assert_perror(!videoCapture.isOpened() && "Cannot open camera.");

        cv::VideoWriter videoWriter;

        /*
         * In this infinite loop keylogger reads
         * pressed keyboard keys.
         * If key Q is pressed program ends.
         * If key Space is pressed program starts capturing video.
         */
        while (true) {
            read(fd, &ev, sizeof(struct input_event));

            // End of program
            if (ev.type == 1 && ev.code == KEY_Q) {
                std::cerr <<"Quit!\n";
                exit(0);
            }

            if (ev.type == 1 && ev.code == KEY_SPACE) {
                if (recording) {
                    std::cerr << "Video is recording now!\n";
                } else {
                    std::cerr << "Starting recording!\n";
                    recording = true;

                    // Generating name of video by date and time
                    std::string video_name = "hidden-video";
                    std::string full_save_name = VIDEO_PATH + video_name + "-" + get_current_date_time() + ".avi";

                    /*
                     * Instantiating video writer with generated name and parameters:
                     * - FPS: 15
                     * - Resolution: 640 x 480
                     */
                    videoWriter.open(
                            full_save_name,
                            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                            15,
                            cv::Size(640, 480)
                    );
                    assert_perror(!videoWriter.isOpened() && "Cannot open writer.");
                }
            }

            // Capturing video
            if (recording) {
                // Reading frame by video capture and write it to file
                cv::Mat frame;
                assert_perror(!videoCapture.read(frame) && "Cannot read frame.");
                videoWriter.write(frame);
            }
        }
    }
}

void camera_menu() {
    const std::string video_name = "video";
    cv::VideoWriter videoWriter;
    cv::VideoCapture videoCapture(0);

    assert_perror(!videoCapture.isOpened() && "Cannot open camera.");

    clear_console();
    printf("Commands:\n"
           "Empty space - Start video recording\n"
           "H - Commands list\n"
           "E - End capturing video\n"
           "P - Save picture\n"
           "Q - Quit camera mode\n\n");

    /*
     * Infinite loop which shows an open window
     * with current camera image.
     * Here user can see and interact with camera.
     */
    for (; ; ) {
        cv::Mat frame;
        assert_perror(!videoCapture.read(frame) && "Cannot read frame.");
        cv::imshow("Camera Mode", frame);

        /*
         * Program waits for any keyboard keys each 50 ms.
         * If H pressed commands list shows
         * If Space pressed video capturing begins
         * If E video capturing stops
         * If P pressed photo from camera saves
         * If Q pressed quit camera mode
         */
        switch (cv::waitKey(50)) {
            case 'h':
                printf("Commands:\n"
                       "Empty space - Start video recording\n"
                       "H - Commands list\n"
                       "E - End capturing video\n"
                       "P - Save picture\n"
                       "Q - Quit camera mode\n\n");
                break;
            case ' ':
                // If video is not capturing now it starts capturing with generated name
                if (!recording) {
                    recording = true;
                    std::string full_save_name = VIDEO_PATH + video_name + "-" + get_current_date_time() + ".avi";
                    printf("Creating new file with name '%s'.\n", full_save_name.c_str());

                    videoWriter.open(
                            full_save_name,
                            cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                            15,
                            cv::Size(640, 480)
                    );
                    assert_perror(!videoWriter.isOpened() && "Cannot open writer.");
                } else {
                    printf("Video is recording now!\n");
                }
                break;
            case 'e':
                // If video is capturing now the capturing stops and video writes releases
                if (recording) {
                    printf("Ending capturing video.\n");
                    recording = false;
                    videoWriter.release();
                } else {
                    printf("Video is not recording.\n");
                }
                break;
            case 'p':
                // Saving current image captured by camera
                save_frame(frame);
                printf("Picture saved!\n");
                break;
            case 'q':
                // Close all windows, release writer and capture, save current video, then exit camera mode
                cv::destroyAllWindows();
                videoWriter.release();
                videoCapture.release();
                recording = false;
                printf("Quit Camera mode.\n");
                return;
            default:
                break;
        }

        // Save frame if video should be recorded
        if (recording) {
            videoWriter.write(frame);
        }
    }
}

int read_int_from_console() {
    int x;
    std::string str;
    std::cin >> str;
    try {
        x = std::stoi(str);
    } catch (std::invalid_argument const& ex) {
        std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
        return -1;
    } catch (std::out_of_range const& ex) {
        std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
        return -1;
    }
    return x;
}

void app_menu() {
    while (true) {
        clear_console();
        printf("Choose option:\n"
               "1 - Print webcams info\n"
               "2 - Open Camera mode\n"
               "4 - Exit\n\n");

        switch (read_int_from_console()) {
            case 1:
                print_webcams_info();
                break;
            case 2:
                camera_menu();
                break;
            case 4:
                return;
            default:
                printf("Unknown option\n");
                break;
        }
        printf("Press any key to continue.");
        std::cin.get();
        std::cin.get();
        clear_console();
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--hidden") == 0) {
        hidden_camera_menu();
    } else {
        app_menu();
    }

    return 0;
}
