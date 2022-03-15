#include "fonctions_tomo_diff.h"

void get_standard_coordinates(Mat& output, int npix, float pixel_size) {
	output = Mat(1, npix, CV_32F);
	for(int i=0; i<npix; i++) {
		output.at<float>(0, i) = pixel_size * ( i - 0.5 * (npix-1) ) ;
	} 
}

void get_fft_compatible_coordinates(Mat& output, int npix, float pixel_size) {
	output = Mat(1, npix, CV_32F);
	for(int i=0; i<npix; i++) {
		output.at<float>(0, i) = pixel_size * ( i - ceil( 0.5 * (npix-1) )) ;
	} 
}

void fov_extension(Mat& x_ext, const Mat& x, float fov_extension_factor, bool flag_transp, Scalar ext_val) {

	//CV_Assert( fov_extension_factor >= 1);

	if( !flag_transp ) {
		float vert = x.rows * (fov_extension_factor - 1) / 2;
		float horz = x.cols * (fov_extension_factor - 1) / 2;
		copyMakeBorder(x, x_ext, ceil(vert), floor(vert), ceil(horz), floor(horz), BORDER_CONSTANT, ext_val );
	}
	else {
		fov_extension_factor = 1.f / fov_extension_factor;
		Size2f s (x.cols * fov_extension_factor, x.rows * fov_extension_factor);
		Point2f p ( x.cols *(1-fov_extension_factor) / 2, x.rows * (1-fov_extension_factor) / 2);
		x_ext = Mat(x, Rect(p, s));
	}
}

void get_propagator_tilt(Mat& H, double z, const Vec3d& k_vect, double n0, Pix pix, double NA) {
	double k_num = norm(k_vect);
	//cout << k_num << endl;
	double lambda_n = (2*PI)/ k_num;


	// [ fx_2D, fy_2D ] = meshgrid(...)
	Mat meshgrid_fx, meshgrid_fy;
	get_fft_compatible_coordinates(meshgrid_fx, pix.nb_x, 1/(pix.dx*pix.nb_x));
	add( meshgrid_fx, k_vect[1]/(2*PI), meshgrid_fx);
	get_fft_compatible_coordinates(meshgrid_fy, pix.nb_y, 1/(pix.dy*pix.nb_y));
	add( meshgrid_fy, k_vect[0]/(2*PI), meshgrid_fy);

	Mat fx_2D;
	repeat(meshgrid_fx, meshgrid_fx.cols, 1, fx_2D);

	transpose(meshgrid_fy, meshgrid_fy);
	Mat fy_2D;
	repeat(meshgrid_fy, 1, meshgrid_fy.rows, fy_2D);

	// lambda2_f2 = ((lambda_n)^2)*(fx_2D.^2+fy_2D.^2) 
	Mat lambda_f2;
	pow(fx_2D, 2, fx_2D);
	pow(fy_2D, 2 , fy_2D);
	Mat temp;
	add(fx_2D, fy_2D, temp);
	//multiply(temp, 1, lambda_f2, pow(lambda_n,2));
	lambda_f2 = temp * pow(lambda_n,2);
	//cout << "lambda_f2\n" << lambda_f2 << endl;

	//imshow("lambda_f2", lambda_f2);
	//waitKey(0);

	Mat ewald;
	if(NA > 0.0)
		// ewald = (lambda2_f2<NA)
		compare(lambda_f2, NA*NA, ewald, CMP_LT);
	else
		ewald = Mat::ones(lambda_f2.size(), CV_8U);

	//imshow("ewald", ewald);
	//waitKey(0);

	//std::cout << "Non zero: " << countNonZero(ewald) << endl;
	if(z == 0) {
		H = Mat_<Complexe>::ones(pix.nb_x, pix.nb_y);
	}
	else {
		// tf = exp(2*1i*pi*z/lambda_n*(1-lambda2_f2).^0.5)
		Mat tf;
		Mat temp;
		subtract(1, lambda_f2, temp);
		sqrt(temp,temp);
		temp = 2 * PI * z / lambda_n  * temp;

		Mat cplx[2] = {Mat(temp.size(), temp.type()) , Mat(temp.size(), temp.type())} ;


		polarToCart( Mat(), temp, cplx[0], cplx[1]);
		merge(cplx, 2, tf);

		Mat tf_filtre = Mat::zeros(tf.size(), tf.type());
		copyTo(tf, tf_filtre, ewald);

		ifftShift(H, tf_filtre);
	}
}

void propagate(Mat& xconv, const Mat& x, const Mat& h, bool flag_pad, const Complexe& ext_val) {
		if(flag_pad)
			fov_extension(xconv, x, 2, false, Scalar(ext_val.real(), ext_val.imag()) );	
		else 
			x.copyTo(xconv);

		ifftShift(xconv, xconv);
		//multiply(xconv, h, xconv);

		// imshow_Complexe("xconv apres fov_extension",xconv); waitKey(0);
		dft(xconv, xconv);
		mulSpectrums(xconv, h, xconv, 0);
		idft(xconv, xconv);
		xconv /= (xconv.rows*xconv.cols);
		ifftShift(xconv, xconv);

		if(flag_pad)
			fov_extension(xconv, xconv, 2, true);
}