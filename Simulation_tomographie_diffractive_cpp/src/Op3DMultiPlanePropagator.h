#ifndef Op3DMULTIPLANEPROPAGATOR_H
#define Op3DMULTIPLANEPROPAGATOR_H

#include "fonctions_tomo_diff.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

// Classe permettant d'utiliser l'opérateur de propagation multi-planes, sans devoir resaisir les mêmes paramètres
class Op3DMultiPlanePropagator {
	public:
	// DIMENSIONAL PARAMETERS
		Vec2i sizepad; 				// padded dimensions (lateral dimensions (x,y) only)
		Vec3d spl;					// sampling steps : [dy,dx,dz] (m)
		float ratio_ext;			// extension factor
		bool flag_ext = false;		// extension required ? (default : false)
        
        float ratio_pad;           	// padding factor
        bool flag_pad = false ;  	// padding required ? (default : false)

    // INSTRUMENTAL PARAMETERS
        float n_0;                 	// refractive index of the medium
        float NA;                  	// numerical aperture
        float z_s;             		// depth of propagation (m)
        float virtual_z_s;     		// virtual depth of propagation (m)
        Vec3d k_vect;          		// list of the illumination wave vectors: [k_y, k_x, k_z]
        float k_n_0;           		// list of wave numbers k_n_0
        float lambda;          		// list of illumination wavelengths

    // OPERATOR PARAMETERS
        int type_prop;         			// plane-to-plane propagation model type
        bool flag_illu = false;         // flag for tiled illum.

        float pad_val = 1.0 ;			// padding value (0 for indexes 'RI' and delta transmittances 'DELTA_TR' ; 1 for transmittances (default))

    // INTERNAL PARAMETERS
        Mat Hdz;                             // inter-plane propagators MTFs
        Mat Hzs;                             // free propagators to the sensor plane MTFs
        Complexe norm_uinc;                     // phase on sensor normalization factor (phase = 0 on the sensor plane)

        Vox vox;                            
        Pix pix, pixPadded;


    public:
        // Constructeur qui retient tous les paramètres passés en argument, et calcule 
        //les noyaux de propagation (Hdz,Hzs)
        Op3DMultiPlanePropagator(Vox _szin, Pix _szout, float _ratio_ext, float _ratio_pad, Vec3d _k_vect
                                , float _n_0, float _z_s, float _NA);

        // Réalise la propagation multi-planes
        // E:   t: matrice 3D, représentant l'échantillon
        // E/S: u_tot_a: matrice de la propagation finale
        //      field_u: tableau de matrices représentant la propagation à chaque étape
        void computeProjectionA(Mat& u_tot_a, vector<Mat>& field_u, const vector<Mat>& t);    
};

#endif // Op3DMULTIPLANEPROPAGATOR_H