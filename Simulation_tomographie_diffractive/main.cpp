#include "Op3DMultiPlanePropagator.h"

// $ Op3DMultiPlanePropagator nom_test z theta phi r_bead lambda n_objet n_0 nb_p dpix NA
int main(int argc, char** argv) {

	string nom_test = "out";
	float z = 10e-6; //dpix*nb_p/2;
	float thetad = 0;
	float phid = 0;
	float r_bead = 2.5e-6;
	float lambda = 632e-09;
	float n_objet = 1.45;
	float n_0 = 1.5190;
	int nb_p = 128;
	float dpix = 4.514280000000000e-07;
	float NA = 0.9;

	// Récupération des paramètres
	for(int i=1; i<argc; i++) {
		switch(i) {
			case 1: nom_test = argv[i]; break;
			case 2: z = atof(argv[i]); break;
			case 3: thetad = atof(argv[i]); break;
			case 4: phid = atof(argv[i]); break;
			case 5: r_bead = atof(argv[i]); break;
			case 6: lambda = atof(argv[i]); break;
			case 7: n_objet = atof(argv[i]); break;
			case 8: n_0 = atof(argv[i]); break;
			case 9: nb_p = atoi(argv[i]); break;
			case 10: dpix = atof(argv[i]); break;
			case 11: NA = atof(argv[i]); break;
		}
	}

	cout << "nom_test:\t " << nom_test << endl;
	cout << "z:\t " << z << endl;
	cout << "thetad:\t " << thetad << endl;
	cout << "phid:\t " << phid << endl;
	cout << "r_bead:\t " << r_bead << endl;
	cout << "lambda:\t" << lambda << endl;
	cout << "n_objet:\t" << n_objet << endl;
	cout << "n_0:\t" << n_0 << endl;
	cout << "nb_p:\t" << nb_p << endl;
	cout << "dpix:\t" << dpix << endl;
	cout << "NA:\t" << NA << endl;


	float theta = thetad *PI/180;
	float phi = phi *PI/180;
	float kn0 = 2*PI*n_0/lambda;
	int r_bead_px = r_bead / dpix;
	Pix pix = {nb_p, nb_p , dpix, dpix};
	Vox vox = {nb_p,nb_p,nb_p ,dpix,dpix,dpix};

	// Calcul du vecteur d'illumination
	Vec3d k_vect = kn0 * Vec3d(	sin(theta)*sin(phi), sin(theta)*cos(phi), cos(theta) );

	// Appel du constructeur: il mémorise les paramètres et calcule les noyaux de propagation
	Op3DMultiPlanePropagator H(vox, pix, 1, 2, k_vect, n_0, z, NA); 

	// L'échantillon, qui est une sphère, représentée par des 0 et 1
	vector<Mat> t = get_bead(vox, {nb_p/2,nb_p/2,nb_p/2}, r_bead_px);

	// L'échantillon est maintenant converti de 0 et 1 en plan de transmittance
	for(int i=0; i<t.size(); i++) {
		Mat _temp[2] = { Mat::zeros(nb_p,nb_p, CV_32F), Mat::zeros(nb_p,nb_p, CV_32F) }; 
		polarToCart(Mat(),  t[i] * 2*PI/lambda* ( n_objet-n_0) * dpix, _temp[0], _temp[1]);

		merge( _temp, 2, t[i]);
		//imshow_Complexe("t", t[i], C_PHAS);
		//waitKey(0);
	}

	Mat u_tot_a; // Matrice après la propagation finale
	vector<Mat> field_u; // Tableau des matrices de toutes les propations
	field_u = vector<Mat>(vox.nb_z);

	// On effectue la simulation
	H.computeProjectionA(u_tot_a, field_u, t);


	// Visulisation/Enregistrement des résultats

/*
	imshow_Complexe("Hdz", H.Hdz, C_PHAS);
	imshow_Complexe("Hzs", H.Hzs, C_PHAS);
	imshow_Complexe("u_tot_a", u_tot_a, C_AMPL);
	waitKey(0);
*/

	unsigned char success = imwrite_Complexe("u_tot_a-" + nom_test, u_tot_a, C_AMPL | C_PHAS, false
		, "Test/");
	cout << success << endl;
	if(success == (C_AMPL|C_PHAS) ) {
		cout << "Image written succesfully" << endl;
	}
	else {
		cout << "An error has occured while saving the image" << endl;
	}
//	imwrite_Complexe("field_u", field_u); // A faire, sauvegarder une matrice 3D

	return 0;
}
