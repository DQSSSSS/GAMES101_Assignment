//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
       return getColorBilinear(u, v);

        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v) {
        float y = u * width;
        float x = (1 - v) * height;

        int x0 = x, x1 = x + 1;
        int y0 = y, y1 = y + 1;

        auto get = [&](cv::Vec3b color) {
            return Eigen::Vector3f(color[0], color[1], color[2]);
        };

        auto c00 = get(image_data.at<cv::Vec3b>(x0, y0));
        auto c01 = get(image_data.at<cv::Vec3b>(x0, y1));
        auto c10 = get(image_data.at<cv::Vec3b>(x1, y0));
        auto c11 = get(image_data.at<cv::Vec3b>(x1, y1));
        
        // x \in [0,1], f(0)=y0, f(1)=y1
        auto lerp = [&](float x, Eigen::Vector3f y0, Eigen::Vector3f y1) {
            return y0 + x*(y1-y0);
        };

        auto s0 = lerp(y-y0, c00, c01);
        auto s1 = lerp(y-y0, c10, c11);
        return lerp(x-x0, s0, s1);
    }

};
#endif //RASTERIZER_TEXTURE_H
