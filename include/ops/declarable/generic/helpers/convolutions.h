//
// Based on PyTorch - https://github.com/pytorch/pytorch
//

#ifndef LIBND4J_CONVOLUTIONS_H
#define LIBND4J_CONVOLUTIONS_H

#include <NDArray.h>

namespace nd4j {
    namespace ops {

        template <typename T>
        class ConvolutionUtils {
        public:
            static Nd4jIndex convsize(Nd4jIndex x, Nd4jIndex k, Nd4jIndex s, const char* vf);

            static Nd4jStatus conv3D(T* output_data, T alpha, T* ptr_input, Nd4jIndex nInputDepth, Nd4jIndex nInputRows, Nd4jIndex nInputCols, T* ptr_weight, Nd4jIndex nKernelDepth, Nd4jIndex nKernelRows, Nd4jIndex nKernelCols, Nd4jIndex sdepth, Nd4jIndex srow, Nd4jIndex scol, const char *vf, const char *xc);

            static Nd4jStatus conv3Dmv(NDArray<T>* r_, T beta, T alpha, NDArray<T>* t_, NDArray<T>* k_, Nd4jIndex sdepth, Nd4jIndex srow, Nd4jIndex scol, const char *vf, const char *xc);

            static void fullXCorr3Dptr(T*r_, T alpha, T *t_, Nd4jIndex it, Nd4jIndex ir, Nd4jIndex ic, T *k_, Nd4jIndex kt, Nd4jIndex kr, Nd4jIndex kc, Nd4jIndex st, Nd4jIndex sr, Nd4jIndex sc);

            static void fullConv3Dptr(T*r_, T alpha, T *t_, Nd4jIndex it, Nd4jIndex ir, Nd4jIndex ic, T *k_, Nd4jIndex kt, Nd4jIndex kr, Nd4jIndex kc, Nd4jIndex st, Nd4jIndex sr, Nd4jIndex sc);

            static void validXCorr3Dptr(T*r_, T alpha, T *t_, Nd4jIndex it, Nd4jIndex ir, Nd4jIndex ic, T *k_, Nd4jIndex kt, Nd4jIndex kr, Nd4jIndex kc, Nd4jIndex st, Nd4jIndex sr, Nd4jIndex sc);

            static void validConv3Dptr(T*r_, T alpha, T *t_, Nd4jIndex it, Nd4jIndex ir, Nd4jIndex ic, T *k_, Nd4jIndex kt, Nd4jIndex kr, Nd4jIndex kc, Nd4jIndex st, Nd4jIndex sr, Nd4jIndex sc);

            static void _dilatedMaxPool3D(T *input_p, T *output_p, T *indz_p, Nd4jIndex nslices, Nd4jIndex itime, Nd4jIndex iwidth, Nd4jIndex iheight, Nd4jIndex otime, Nd4jIndex owidth, Nd4jIndex oheight, int kT, int kW, int kH, int dT, int dW, int dH, int pT, int pW, int pH, int dilationT, int dilationW, int dilationH);

            static void _dilatedMaxPool3D_bp(T *gradInput_p, T *gradOutput_p, T *indz_p, Nd4jIndex nslices, Nd4jIndex  itime, Nd4jIndex  iwidth, Nd4jIndex  iheight, Nd4jIndex otime, Nd4jIndex owidth, Nd4jIndex oheight, int dT, int dW, int dH, int pT, int pW, int pH, int dilationT, int dilationW, int dilationH);

            static void _avgPool3D(T *input_p, T *output_p, Nd4jIndex iC, Nd4jIndex iD, Nd4jIndex iH, Nd4jIndex iW, Nd4jIndex oD, Nd4jIndex oH, Nd4jIndex oW, int kD, int kH, int kW, int sD, int sH, int sW, int pD, int pH, int pW, bool count_include_pad);

            static void _avgPool3D_bp(T *gradI_p, T *gradO_p, Nd4jIndex iC, Nd4jIndex iD, Nd4jIndex iH, Nd4jIndex iW, Nd4jIndex oD, Nd4jIndex oH, Nd4jIndex oW, int kD, int kH, int kW, int sD, int sH, int sW, int pD, int pH, int pW, bool count_include_pad);
            
            // vol [volC, volD, volH, volW], col [volC*kD*kW*kH, colD*colH*colW]
            static void vol2col(NDArray<T>& vol, NDArray<T>& col, const int colD, const int colH, const int colW, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // volume [bS, volC, volD, volH, volW], columns [bS, volC, kD, kH, kW, colD, colH, colW]
            static void vol2col2(NDArray<T>& vol, NDArray<T>& col, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // col [bS, volC, kD, kH, kW, colD, colH, colW], vol [bS, volC, volD, volH, volW]
            static void col2vol2(NDArray<T>& col, NDArray<T>& vol, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // col [volC*kD*kW*kH, colD*colH*colW], vol [volC, volD, volH, volW]
            static void col2vol(NDArray<T>& vol, NDArray<T>& col, const int colD, const int colH, const int colW, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            static void _vol2col(const T *data_vol, const int channels, const int depth, const int height, const int width, const int kT, const int kH, const int kW, const int pT, const int pH, const int pW, const int dT, const int dH, const int dW, const int dilationT, const int dilationH, const int dilationW, T *data_col);

            static void _col2vol(const T* data_col, const int channels, const int depth, const int height, const int width, const int out_depth, const int out_height, const int out_width, const int kT, const int kH, const int kW, const int pT, const int pH, const int pW, const int dT, const int dH, const int dW, const int dilationT, const int dilationH, const int dilationW, T* data_vol);

            static void calcOutSizePool2D(int& oH, int& oW, const int kH, const int kW, const int sH, const int sW, const int pH, const int pW, const int dH, const int dW, const int iH, const int iW, const int isSameMode);

            static void calcOutSizePool3D(int& oD, int& oH, int& oW, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW, const int iD, const int iH, const int iW, const int isSameMode);

            static void _calcPadding2D(int& pH, int& pW, int oH, int oW, int inH, int inW, int kH, int kW, int sH, int sW, int dH, int dW);

            static void calcPadding3D(int& pD, int& pH, int& pW, const int oD, const int oH, const int oW, const int iD, const int iH, const int iW, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int dD, const int dH, const int dW);

            static void _im2col(const T* data_im, const int channels, const int height, const int width, const int kernel_h, const int kernel_w, const int pad_h, const int pad_w, const int stride_h, const int stride_w, const int dilation_h, const int dilation_w, T* data_col);            

            // input [bS, iC, iD, iH, iW], output [bS, iC, oD, oH, oW]
            static void maxPool3dFrame(NDArray<T>& input, NDArray<T>& output, const int iStride, const int oStride, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // input [bS, iC, iD, iH, iW], indices [bS, iC, oD, oH, oW]
            static void maxPool3dIndicesFrame(NDArray<T>& input, int* indices, const int iStride, const int indStride, const int oD, const int oH, const int oW, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // input [bS, iC, iD, iH, iW], indices [bS, iC, iD, iH, iW], output [bS, iC, oD, oH, oW]
            static void maxPool3dFrameBp(NDArray<T>& input, const int* indices, NDArray<T>& output, const int iStride, const int oStride, const int kD, const int kH, const int kW, const int sD, const int sH, const int sW, const int pD, const int pH, const int pW, const int dD, const int dH, const int dW);

            // calculation of output height and width in 2D deconvolution procedure
            static void calcOutSizeDeconv2D(int& oH, int& oW, const int kH, const int kW, const int sH, const int sW, const int pH, const int pW, const int dH, const int dW, const int iH, const int iW, const int isSameMode);

        
        };
    }
}

#endif //LIBND4J_CONVOLUTIONS_H
