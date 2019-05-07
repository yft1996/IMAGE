/*
 * @功能      image.hpp内TODO函数实现
 * @姓名      杨丰拓
 * @日期      2019-4-29
 * @时间      17:14
 * @邮箱
*/
#ifndef _IMAGE_1_CUH_
#define _IMAGE_1_CUH_



/**
 * @func            填充图像
 * @property        每个颜色通道赋相同的值,值取自color数组
 * @param_out       image           填充后的图像
 * @param_in        color           颜色数组
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        color_size      color数组大小
 * @param_in        contrast        cpu实现,与gpu实现对比
 * @return
 * 调用示例：
 * fill_color_by_cuda(&src1.at(0),color,src1.width(),src1.height(),src1.channels(),3,&src.at(0));
 */
template <typename T>
int fill_color_by_cuda(T *image,T *color,int const w,int const h,int const c,int const color_size,T *contrast);

template <>
int fill_color_by_cuda<char>(char *image,char *color,int const w,int const h,int const c,int const color_size,char *contrast);

template <>
int fill_color_by_cuda<float>(float  *image,float *color,int const w,int const h,int const c,int const color_size,float *contrast);


/**
 * @func            添加颜色通道
 * @property        为已存在图像添加一个新的颜色通道
 * @param_out       dst_image       添加颜色通道后的图像
 * @param_in        src_image       已存在图像
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        num_channels    所要添加的通道数
 * @param_in        value           指向添加的颜色通道内所要赋的值数组
 * @param_in        contrast        cpu实现,与gpu实现对比
 * @return
 * 调用示例：
 * add_channels_by_cuda(&dst.at(0),&src1.at(0),src1.width(),src1.height(),src1.channels(),num_channels,value,&src.at(0));
 */
template <typename T>
int add_channels_by_cuda(T *dst_image,T  * src_image,int const w,int const h, int const c, int const num_channels,T * value,T *contrast);
template <>
int add_channels_by_cuda<char>(char *dst_image,char  * src_image,int const w,int const h, int const c, int const num_channels,char * value,char *contrast);
template <>
int add_channels_by_cuda<float>(float *dst_image,float  * src_image,int const w,int const h, int const c, int const num_channels,float * value,float *contrast);


/**
 *
 * @func            添加颜色通道(向后添加/向前添加)
 * @property        为已存在图像添加一个新的颜色通道
 * @param_out       dst_image       添加颜色通道后的图像
 * @param_in        src_image       已存在图像
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        value           vector容器,存放待添加值
 * @param_in        _front_back     bool值,决定向后添加颜色通道(true)还是向前添加颜色通道(flase)
 * @param_in        contrast        cpu实现,与gpu实现对比
 * @return
 * 调用示例:
 * add_channels_front_by_cuda(&dst.at(0),&src1.at(0),src1.width(),src1.height(),src1.channels(),_value,false,&src.at(0));
 */
template <typename T>
int add_channels_front_by_cuda(T *dst_image,T  * src_image,int const w,int const h, int const c, std::vector<T> value,bool _front_back,T *contrast);
template <>
int add_channels_front_by_cuda<char>(char *dst_image,char  * src_image,int const w,int const h, int const c, std::vector<char> value,bool _front_back,char *contrast);
template <>
int add_channels_front_by_cuda<float>(float *dst_image,float  * src_image,int const w,int const h, int const c, std::vector<float> value,bool _front_back,float *contrast);
#endif //_IMAGE_1_CUH_