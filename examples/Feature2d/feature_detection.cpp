///*
// * @desc    sift特征提取检测
// * @author  安帅
// * @date    2019-03-27
// * @email   1028792866@qq.com
//*/
//#include "sift.hpp"
//#include "image_io.h"
//#include "timer.h"
//#include <iostream>
//#include <visualizer.hpp>
//
////自定义排序函数 描述子尺度从大到小排序
//bool scale_compare(image::Sift::Descriptor const& d1, image::Sift::Descriptor const& d2)
//{
//    return d1.scale > d2.scale;
//}
//
//
//int main()
//{
//
//
//    image::ByteImage::ImagePtr image;
//    std::string image_filename = "/home/doing/lena.jpg";
//
//    try
//    {
//        std::cout<<"加载 "<<image_filename<<"中..."<<std::endl;
//        image = image::loadImage(image_filename);
//    }
//    catch (std::exception& e)
//    {
//        std::cerr<<"错误: "<<e.what()<<std::endl;
//        return 1;
//    }
//
//    std::cout<<"================================="<<std::endl;
//    std::cout<<"           Debug专用              "<<std::endl;
//    std::cout<<"================================="<<std::endl;
//
//    image::Sift::Keypoints sift_keypoints;
//    image::Sift::Descriptors sift_descriptors;
//
//    image::Sift::Options sift_options;
//    sift_options.verbose_output = true;
//    sift_options.debug_output = true;
//    image::Sift sift(sift_options);
//    sift.set_image(image);
//
//    image::TimerHigh timer;
//    sift.process();
//    std::cout<<"计算Sift特征用时 "<<timer.get_elapsed()<<" 毫秒"<<std::endl;
//
//    sift_keypoints = sift.get_keypoints();
//    sift_descriptors = sift.get_descriptors();
//
//    //TODO:sort函数 CUDA@杨丰拓
//    std::sort(sift_descriptors.begin(),sift_descriptors.end(),scale_compare);
//
//    std::cout<<"================================="<<std::endl;
//    std::cout<<"           Debug专用              "<<std::endl;
//    std::cout<<"================================="<<std::endl;
//
//    image::Visualizer<uint8_t> sift_vis;
//    Keypoints keypoints;
//    for(int i=0;i<sift_keypoints.size();i++)
//    {
//        KeypointVis pt;
//        pt.x=sift_keypoints[i].x;
//        pt.y=sift_keypoints[i].y;
//        pt.radius=(sift_keypoints[i].sample+1);
//        pt.orientation=sift_keypoints[i].octave;
//        keypoints.push_back(pt);
//    }
//    image::ByteImage::ImagePtr image_out;
//    image_out = sift_vis.drawKeypoints(image,keypoints,image::RADIUS_CIRCLE_ORIENTATION);
//
//    //保存图像 还需要重载
//    std::string image_out_name = "result_sift.png";
//    std::cout<<"保存图像: "<<std::endl;
//    image::saveImage(image_out,image_out_name);
//    return 0;
//}
//
#include <MATH/Flann/flann.hpp>
#include "Features2d/features2d.h"
#include "IMAGE/image_io.h"
#include "timer.h"
#include <iostream>
#include "IMAGE/visualizer.hpp"
#include "IMAGE/image_process.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
using namespace features2d;
using namespace std;
void unpack(const int kptoctave, int& octave, int& layer, float& scale)
{
    octave = kptoctave & 255;
    layer = (kptoctave >> 8) & 255;
    octave = octave < 128 ? octave : (-128 | octave);
    scale = octave >= 0 ? 1.f/(1 << octave) : (float)(1 << -octave);
}
int main()
{
    image::ByteImage::ImagePtr test=image::ByteImage::create(2,2,3);
    image::ByteImage::ImagePtr image=std::make_shared<ByteImage>();
    image::ByteImage::ImagePtr image2=std::make_shared<ByteImage>();
    std::string image_filename = "/home/doing/box2.jpg";
    std::string image_filename2 = "/home/doing/box.jpg";
#define USECV 0
#if USECV
    cv::ImagePtr<cv::Feature2D> cvorb=cv::ORB::create();
    cv::ImagePtr<cv::Feature2D> cvsift = cv::xfeatures2d::SIFT::create();
    vector<cv::KeyPoint> cvkeyPoint, cvkeyPoint2;
    cv::Mat cvdescriptors, cvdescriptors2;
    cv::Mat img = cv::imread(image_filename);
    cv::Mat img2 = cv::imread(image_filename2);
    cvsift->detect(img, cvkeyPoint);
    cvsift->compute(img, cvkeyPoint, cvdescriptors);
    cvsift->detect(img2, cvkeyPoint2);
    cvsift->compute(img2, cvkeyPoint2, cvdescriptors2);
    cout<<cvkeyPoint.size()<<endl;
    cout<<cvkeyPoint2.size()<<endl;
#endif

    cv::Mat cvimg_gray=cv::imread(image_filename,0);
    cv::Mat cvimg_gray2=cv::imread(image_filename2,0);

    image = image::loadImage(image_filename);
    image2 = image::loadImage(image_filename2);

    image::ByteImage::ImagePtr image_gray=image::desaturate<unsigned char>(image,DESATURATE_LUMINANCE);
    image::FloatImage::ImagePtr img_gray_ptr=make_shared<image::Image<float>>(image->width(),image->height(),1);
    converTo(*image_gray,*img_gray_ptr);
    Mat* img_gray=(Mat*)(img_gray_ptr.get());
    double start,end;
    start=clock();
    image::ByteImage::ImagePtr image_gray2=image::desaturate<unsigned char>(image2,DESATURATE_LUMINANCE);
    end=clock();
    cout<<"time: "<<(end-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    image::FloatImage::ImagePtr img_gray_ptr2=make_shared<image::Image<float>>(image2->width(),image2->height(),1);
    converTo(*image_gray2,*img_gray_ptr2);
    //for(int i=0;i<image_gray2->get_value_amount();i++)
     //   img_gray_ptr2->getData()[i]=(float)image_gray2->getData()[i];
    Mat* img_gray2=(Mat*)(img_gray_ptr2.get());


    shared_ptr<features2d::Feature2D> orb = features2d :: ORB :: create();
    shared_ptr<features2d::Feature2D> sift = features2d :: SIFT :: create();
    vector<features2d::KeyPoint> keyPoint,keyPoint2;
    Mat descriptors,descriptors2;
    UCMat mask;
    //orb->detect(*image_gray,keyPoint,mask);
    //orb->compute(*image_gray,keyPoint,descriptors);

    sift->detectAndCompute(*img_gray,mask,keyPoint,descriptors);
    //orb->detectAndCompute(*image_gray,mask,keyPoint,descriptors);

    sift->detectAndCompute(*img_gray2,mask,keyPoint2,descriptors2);
    //orb->detectAndCompute(*image_gray2,mask,keyPoint2,descriptors2);
    cout<<keyPoint.size()<<endl;
    cout<<keyPoint2.size()<<endl;
    image::Visualizer<uint8_t> sift_vis;
    image::Keypoints keypoints;
    float angle=0;
    for(int i=0;i<keyPoint.size();i++)
    {
        image::KeypointVis pt;
        pt.x=keyPoint[i].m_pt.x;
        pt.y=keyPoint[i].m_pt.y;
        pt.orientation=keyPoint[i].m_angle;
        //cout<<pt.orientation<<endl;
        angle+=pt.orientation;
        pt.radius=keyPoint[i].m_response*300;
        keypoints.push_back(pt);
    }
    cout<<angle/keyPoint.size()<<endl;
    image::ByteImage::ImagePtr image_out;
    image_out = sift_vis.drawKeypoints(image,keypoints,image::RADIUS_CIRCLE_ORIENTATION);
    std::string image_out_name = "result_sift1.png";
    image::saveImage(image,image_out_name);

    int nn=1;
#if USECV

    float *data=new float[keyPoint.size()*2];
    float *cvdata=new float[cvkeyPoint.size()*2];
    for(int i=0;i<keyPoint.size();i++)
    {
        data[i*2]=keyPoint[i].pt.x;
        data[i*2+1]=keyPoint[i].pt.y;
    }
    for(int i=0;i<cvkeyPoint.size();i++)
    {
        cvdata[i*2]=cvkeyPoint[i].pt.x;
        cvdata[i*2+1]=cvkeyPoint[i].pt.y;
    }
    flann::Features<float> dataset=flann::Features<float>(data,keyPoint.size(),2);
    flann::Features<float> query=flann::Features<float>(cvdata,cvkeyPoint.size(),2);
#else
    flann::Features<float> dataset=flann::Features<float>(descriptors.getData().data(),descriptors.rows(),descriptors.cols());
    flann::Features<float> query=flann::Features<float>(descriptors2.getData().data(),descriptors2.rows(),descriptors2.cols());
#endif
    flann::Features<int> indices(new int[query.rows*nn], query.rows, nn);
    flann::Features<float> dists(new float[query.rows*nn], query.rows, nn);
    flann::Index<flann::L2<float> > index(dataset, flann::KDTreeIndexParams());
    index.buildIndex();
    index.knnSearch(query, indices, dists, nn, flann::SearchParams(128));
#if USECV
    for(int i=0;i<cvkeyPoint.size();i++)
    {
        int octave =cvkeyPoint[i].octave;
        cout<<(int)cvkeyPoint[i].pt.x<<" "<<(int)cvkeyPoint[i].pt.y<<" "<<(int)cvkeyPoint[i].angle<<" "<<octave<<" "<<cvkeyPoint[i].response<<" "<<cvkeyPoint[i].size<<endl;
        int k=indices[i][0];
        octave =keyPoint[i].octave;
        cout<<(int)keyPoint[k].pt.x<<" "<<(int)keyPoint[k].pt.y<<" "<<(int)keyPoint[k].angle<<" "<<octave<<" "<<keyPoint[k].response<<" "<<keyPoint[k].size<<endl;
        cout<<endl;
    }
    return 0;
#endif
    float min_dist=0;
    for(int i=0;i<dists.rows;i++)
        min_dist+=dists[i][0];
    min_dist/=dists.rows;
    image::ByteImage::ImagePtr res=make_shared<image::ByteImage>(image::ByteImage(image->width()*2,image->height(),3));
    for(int i=0;i<image->height();i++){
        int offset=i*image->width()*image->channels();
        int offset2=i*res->width()*res->channels();
        for(int j=0;j<image->width()*image->channels();j++){
            res->at(offset2+j)=image->at(offset+j);
            res->at(offset2+image->width()*image->channels()+j)=image2->at(offset+j);
        }
    }

    int num=0;
    for(int i=0;i<keyPoint2.size();i++)
    {
        uint8_t color[3]={(uint8_t)(rand()%255),(uint8_t)(rand()%255),(uint8_t)(rand()%255)};
        if(dists[i][0]<min_dist*0.65) {
            sift_vis.drawLine(*res, keyPoint[indices[i][0]].m_pt.x, keyPoint[indices[i][0]].m_pt.y,
                               keyPoint2[i].m_pt.x + image->width(), keyPoint2[i].m_pt.y, color);
            num++;
        }
    }
    cout<<num<<endl;

    //保存图像 还需要重载
    image_out_name = "result_sift3.png";
    std::cout<<"保存图像: "<<std::endl;
    image::saveImage(res,image_out_name);
    return 0;
}