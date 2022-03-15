#ifndef UTILITAIRES_H
#define UTILITAIRES_H

//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <complex>
#include <stdlib.h>
#include <iostream>
//#include <chrono>

using namespace std;
using namespace cv;

#define PI 3.1415926535897932384626433

typedef complex<float> Complexe;

enum Composants {C_REEL = 0b0001, C_IMAG = 0b0010, C_AMPL = 0b0100, C_PHAS = 0b1000, C_TOUS = 0b1111};

// Structure indiquant les dimensions (en px) et l'échelle (en m/px) de l'échantillon 3D
struct Vox {
	int nb_x;
	int nb_y;
	int nb_z;

	float dx;
	float dy;
	float dz;
};

// Structure indiquant les dimensions (en px) et l'échelle (en m/px) de 
struct Pix {
	int nb_x;
	int nb_y;

	float dx;
	float dy;
};

// --Fonction configuré pour une machine Linux
// Ouvrir une image avec un visualisateur
// E: 	file : string, chemin complet du fichier
//		background : booléen, vrai si le programme continue son éxecution après avoir lancé le visualisateur
void imshow_viewer(const string& file, bool background = true);

// Afficher les composantes d'une matrice complexe avec cv::imshow()
// E: 	name : nom de l'objet
// 		m : matrice de complexes
//		composants : entier indiquant bit-à-bit les composantes de l'image (voit enum Composants)
// Exemple pour afficher la partie réelle et l'amplitude: 
//		imshow_Complexe("Titre de la fenètre", matrice_avec_deux_chaines, C_REEL | C_AMPLITUDE);
void imshow_Complexe(const string& name, const Mat_<Complexe>& m, int composants = C_REEL );

// Ecrit les composantes d'une matrice complexe avec cv::imwrite() en format TIFF, et l'ouvre avec imshow_viewer()
// E: 	name : nom de l'objet
// 		m : matrice de complexes
//		composants : entier indiquant bit-à-bit les composantes de l'image à afficher (voit enum Composants)
//		open : booléen, ouvre l'image si vrai
//		path : chemin du répertoire où s'enregistre l'image
// S: 	entier indiquant bit-à-bit les composantes de l'image qui ont été enregistré avec succés
unsigned char imwrite_Complexe(const string& name, const Mat& m, int composants = C_AMPL, bool open = true, string path = "/tmp/images/");

// Fonction retournant un échantillon (représentée par une matrice 3D de réels), contenant une sphère
// E: 	vox: Dimensions et échelle de l'échantillon
//		pos: centre de la sphère (en px)
//		r_bead: rayon de la sphère (en px)
// S: Vecteur de Matrices
vector<Mat> get_bead(Vox vox, const Vec3i& pos, float r_bead);

// Inverse les quadrants d'une matrice, utilisée pour la FFT
// E: 	in: Matrice
// E/S: out: Matrice
void ifftShift(Mat& out, Mat& in);

#endif // UTILITAIRES_H