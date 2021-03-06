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
int fillColorByCuda(T *image,T *color,int const w,int const h,int const c,int const color_size);

template <>
int fillColorByCuda(char *image,char *color,int const w,int const h,int const c,int const color_size);

template <>
int fillColorByCuda(float  *image,float *color,int const w,int const h,int const c,int const color_size);


/**
 * @func            添加颜色通道
 * @property        为已存在图像添加一个新的颜色通道
 * @param_out       dst_image       添加颜色通道后的图像
 * @param_in        src_image       已存在图像
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        num_channels    所要添加的通道数
 * @param_in        value           指向添加的颜色通道内所要赋的值
 * @param_in        contrast        cpu实现,与gpu实现对比
 * @return
 * 调用示例：
 * addChannelsByCuda(&dst.at(0),&src1.at(0),src1.width(),src1.height(),src1.channels(),num_channels,value);
 */
template <typename T>
int addChannelsByCuda(T *dst_image,T  * src_image,int const w,int const h, int const c, int const num_channels,T  value);
template <>
int addChannelsByCuda(char *dst_image,char  * src_image,int const w,int const h, int const c, int const num_channels,char  value);
template <>
int addChannelsByCuda(float *dst_image,float  * src_image,int const w,int const h, int const c, int const num_channels,float  value);


/**
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
 * add_channels_front_by_cuda(&dst.at(0),&src1.at(0),src1.width(),src1.height(),src1.channels(),_value,false);
 */
template <typename T>
int addChannelsFrontByCuda(T *dst_image,T  * src_image,int const w,int const h, int const c, std::vector<T> value,bool _front_back);
template <>
int addChannelsFrontByCuda(char *dst_image,char  * src_image,int const w,int const h, int const c, std::vector<char> value,bool _front_back);
template <>
int addChannelsFrontByCuda(float *dst_image,float  * src_image,int const w,int const h, int const c, std::vector<float> value,bool _front_back);

/**
 * @func            交换颜色通道
 * @property        交换输入图像的两个颜色通道
 * @param_in_out    src             待交换图像
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        swap_c1         参与交换的颜色通道1
 * @param_in        swap_c2         参与交换的颜色通道2
 * @param_in        contrast        cpu实现,与gpu实现对比
 * @return
 * 调用示例:
 * swapChannelsByCuda(&src_gpu.at(0),src_gpu.width(),src_gpu.height(),src_gpu.channels(),0,2);
 */
template <typename T>
int swapChannelsByCuda(T *src,int const w,int const h,int c,int const swap_c1,int swap_c2);
template <>
int swapChannelsByCuda(char *src,int const w,int const h,int c,int const swap_c1,int swap_c2);
template <>
int swapChannelsByCuda(float *src,int const w,int const h,int c,int const swap_c1,int swap_c2);


/**
 * @func            复制颜色通道
 * @property        复制输入图像的两个颜色通道
 * @param_in_out    image           输入图像
 * @param_in        w               图像宽
 * @param_in        h               图像高
 * @param_in        c               图像颜色通道
 * @param_in        copy_c          被复制的颜色通道
 * @param_in        paste_c         目的颜色通道
 * @return
 * 调用示例:
 * copyChannelsByCuda(&src_gpu.at(0),src_gpu.width(),src_gpu.height(),src_gpu.channels(),copy_c,paste_c);
 */
template <typename T>
int copyChannelsByCuda(T *image,int const w,int const h,int const c,int const copy_c,int const paste_c);
template <>
int copyChannelsByCuda(char *image,int const w,int const h,int const c,int const copy_c,int const paste_c);
template <>
int copyChannelsByCuda(float *image,int const w,int const h,int const c,int const copy_c,int const paste_c);

/**
 * @func            删除颜色通道
 * @property        删除输入图像的某个颜色通道(del_c)
 * @param_out       dstImage          删除颜色通道后的图像
 * @param_in        srcImage          输入图像
 * @param_in        src_w             图像宽
 * @param_in        src_h             图像高
 * @param_in        src_c             图像颜色通道
 * @param_in        del_c             需删除的颜色通道
 * @return
 * 调用示例:
 * deleteChannelByCuda(&dst_gpu.at(0),&src_gpu.at(0),src_gpu.width(),src_gpu.height(),src_gpu.channels(),del_c);
 */
template <typename T>
int deleteChannelByCuda(T *dstImage,T *srcImage,int const src_w,int const src_h,int const src_c,int const del_c);
template <>
int deleteChannelByCuda(char *dstImage,char *srcImage,int const src_w,int const src_h,int const src_c,int const del_c);
template <>
int deleteChannelByCuda(float *dstImage,float *srcImage,int const src_w,int const src_h,int const src_c,int const del_c);
#endif //_IMAGE_1_CUH_