#include "utilitaires.h"

void imshow_viewer(const string& file, bool background) {
	static const string viewer = "imagej -p 1 "; // L'application à utiliser
	char bg = background ? '&' : ' '; 
	string no_output = " > /dev/null 2>&1 "; // Redirige la sortie de l'application, pour ne pas afficher de messages dans la console
	string cmd = viewer + '\"' + file + '\"' + no_output + bg; // La commande Linux
	cout << "Executing command: " << cmd << endl;
	system( cmd.c_str() ) ;
}

void imshow_Complexe(const string& name, const Mat_<Complexe>& m, int composants) {
	Mat cart[2];
	//MatCplx_to_4Mat(m, comp);
	split(m, cart);

	Mat temp;

	if(composants & C_REEL) {	
		normalize(cart[0], temp, 0, 1, NORM_MINMAX);
		imshow(name + " - Reel", temp);
	}
	if(composants & C_IMAG) {
		normalize(cart[1], temp, 0, 1, NORM_MINMAX);
		imshow(name + " - Imaginaire", temp);
	}
	if(composants & C_AMPL) {
		magnitude(cart[0], cart[1], temp);
		normalize(temp, temp, 0, 1, NORM_MINMAX);
		imshow(name + " - Amplitude", temp);	
	}
	if(composants & C_PHAS) {
		phase(cart[0], cart[1], temp);
		temp /= 2*PI;
		imshow(name + " - Phase", temp);
	}
}

unsigned char imwrite_Complexe(const string& name, const Mat& m, int composants, bool open, string path) {
	static const string format = ".tiff";
	string file_reel = path+"/"+name+"-reel"+format;
	string file_imag = path+"/"+name+"-imag"+format;
	string file_ampl = path+"/"+name+"-ampl"+format;
	string file_phas = path+"/"+name+"-phas"+format;

	system( ("mkdir -p " + path).c_str() ); // Commande pour linux

	Mat cart[2];
	//MatCplx_to_4Mat(m, comp);

	if( m.channels() != 2) {
		cart[0] = m;
		composants = C_REEL;
	}
	else
		split(m, cart);

	Mat temp;
	unsigned char success = 0b0;

	if(composants & C_REEL) {
		//imwrite(file + " - Reel.tiff", cart[0]);
		success |= C_REEL * imwrite( file_reel, cart[0] );
		if(open) imshow_viewer(file_reel);
	}
	if(composants & C_IMAG) {
		success |= C_IMAG * imwrite(file_imag, cart[1]);
		if(open) imshow_viewer(file_reel);
	}
	if(composants & C_AMPL) {
		magnitude(cart[0], cart[1], temp);
		success |= C_AMPL * imwrite(file_ampl, temp);	
		if(open) imshow_viewer(file_ampl);
	}
	if(composants & C_PHAS) {
		phase(cart[0], cart[1], temp);
		success |= C_PHAS * imwrite(file_phas, temp);
		if(open) imshow_viewer(file_phas);
	}

	return success;
}

vector<Mat> get_bead(Vox vox, const Vec3i& pos, float r_bead) {

	vector<Mat> m(vox.nb_z);

	for(int z=0 ; z<vox.nb_z ; z++) {
		m[z] = Mat(vox.nb_x, vox.nb_y, CV_32F);

		for(int x=0 ; x < vox.nb_x ; x++) for(int y=0 ; y<vox.nb_y ; y++)  {
			float v;
			Point3i d (pos - Vec3i(x,y,z));
			if( d.ddot(d) <= r_bead*r_bead )
				v = 1;
			else 
				v = 0;
			m[z].at<float>(x,y) = v;
		}
	}

	return m;
}

void ifftShift(Mat& out, Mat& in) {
//	imshow_Complexe("in", in); waitKey(0);

	int cx = in.cols/2;
	int cy = in.rows/2;

	Rect	topleft(0, 0, cx, cy),
			topright(cx, 0, cx, cy),
			botleft(0, cy, cx, cy),
			botright(cx, cy, cx, cy);
	Mat temp;
	temp = Mat(in.size(), in.type());

	Mat qi0(in, topleft),  qo0(temp, topleft);   
    Mat qi1(in, topright), qo1(temp, topright); 
    Mat qi2(in, botleft),  qo2(temp, botleft); 
    Mat qi3(in, botright), qo3(temp, botright); 

    qi0.copyTo(qo3);
    qi3.copyTo(qo0);
    qi1.copyTo(qo2);
    qi2.copyTo(qo1);

    temp.copyTo(out);
/*
	imshow_Complexe("qi1", qi1);    
    imshow_Complexe("out3", out);

    imshow_Complexe("qo1", qo1);

    waitKey(0);
*/
}