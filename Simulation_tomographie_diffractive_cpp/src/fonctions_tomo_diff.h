#ifndef FONCTIONS_TOMO_DIFF_H
#define FONCTIONS_TOMO_DIFF_H

#include "utilitaires.h"

// Retourne une matrice de une ligne comportant des coordoonéées standards
// E: 	npix: nombre de pixels
//		pixel_size: taille d'un pixel
// E/S:	output: matrice de sortie
void get_standard_coordinates(Mat& output, int npix, float pixel_size);

// Retourne une matrice de une ligne comportant des coordoonéées compatible à la FFT
// E: 	npix: nombre de pixels
//		pixel_size: taille d'un pixel
// E/S:	output: matrice de sortie
void get_fft_compatible_coordinates(Mat& output, int npix, float pixel_size);


// Modifie la taille d'une matrice, pour éviter l'effet de repilement lors d'une fft
// E: 	x: matrice à modifier
//		fov_extension_factor: facteur d'extension, doit être >= 1
//		flag_transp: booléen, faux pour extension de l'image en lui donnant un cadre, vrai pour une réduction
//		ext_val: valeur du cadre si on fait extension (peut être complexe)
// E/S:	x_ext: matrice de sortie
void fov_extension(Mat& x_ext, const Mat& x, float fov_extension_factor, bool flag_transp=false, Scalar ext_val = Scalar(0,0));

// Retourne le noyeau de propagation H
// E:	z: distance de propagation(mètres)
//		k_vect: vecteur d'illumination
//		n0: indice de réfraction du milieu
// 		pix: dimension et échelle de la matrice
// 		NA: ouverture numérique
// E/S:	H: matrice de sortie
void get_propagator_tilt(Mat& H, double z, const Vec3d& k_vect, double n0, Pix pix, double NA = 1.0 );

// Réalise une propagation
// E:	x: matrice de complexes avant 
//		h: noyeau de propagation
//		flag_pad: vrai pour faire une extension de champ avant la FFT
// 		ext_val: valeur du cadre pour l'extension
// E/S:	xconv: matrice de sortie
void propagate(Mat& xconv, const Mat& x, const Mat& h, bool flag_pad = true, const Complexe& ext_val = 0);

// Conversion de std::complex<float> (càd Complexe) à cv::Scalar
inline Scalar cplxToScalar(const Complexe& z) {
	return Scalar(z.real(), z.imag());
}

#endif // FONCTIONS_TOMO_DIFF_H	