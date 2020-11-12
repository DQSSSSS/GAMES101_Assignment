#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;
constexpr double eps = 1e-6;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    float a = rotation_angle / 180 * MY_PI;
    model << cos(a), -sin(a), 0, 0,
             sin(a), cos(a), 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;
    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    float alpha = eye_fov;
    float n = zNear, f = zFar;
    float t = tan(alpha/2) * abs(n), r = t * aspect_ratio;
    float b = -t, l = -r;

    Eigen::Matrix4f p2o;
    p2o << n, 0, 0, 0,
           0, n, 0, 0,
           0, 0, n+f, -n*f,
           0, 0, 1, 0;

    Eigen::Matrix4f translation; 
    translation << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1;

    Eigen::Matrix4f zoom;
    zoom << 2/(r-l), 0, 0, 0,
            0, 2/(t-b), 0, 0,
            0, 0, 2/(n-f), 0,
            0, 0, 0, 1;

    Eigen::Matrix4f o = zoom * translation;
    
    projection = o * p2o;
    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    return projection;
}


Eigen::Matrix4f get_rotation(Eigen::Vector3f axis, float angle) {

   // std::cout << axis << std::endl;

    Eigen::Matrix4f ans = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f one;
    one << 1,0,0, 0,1,0, 0,0,1;

    float alpha = angle / 180 * MY_PI;
    float cs = cos(alpha), sn = sin(alpha);
    float x = axis[0], y = axis[1], z = axis[2];
    Eigen::Matrix3f cross;
    cross << 0,-z,y, z,0,-x, -y,x,0;

    Eigen::Matrix3f tmp = cs*one + (1-cs)*axis*axis.transpose() + sn*cross;

    for(int i = 0;i < 4;i ++) {
        for(int j = 0;j < 4;j ++) {
            if(i < 3 && j < 3)
                ans(i,j) = tmp(i,j);
            else if(i == 3 && j == 3)
                ans(i,j) = 1;
            else
                ans(i,j) = 0;
        }
    }
    return ans;
}


int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    bool any_axis = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    } else if(argc == 2) {
        any_axis = true;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    Eigen::Vector3f axis = {2, 0, -2};
    axis.normalize();

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        if(!any_axis)
            r.set_model(get_model_matrix(angle));
        else 
            r.set_model(get_rotation(axis, angle));
        
        //std::cout << "model_finish" << std::endl;

        r.set_view(get_view_matrix(eye_pos));
       // std::cout << "view_finish" << std::endl;

        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        //std::cout << "projection_finish" << std::endl;

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
       // std::cout << "draw_finish" << std::endl;

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        
      //  std::cout << "convert_finish" << std::endl;

        cv::imshow("image", image);
        
      //  std::cout << "show_finish" << std::endl;

        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
