#include "Op3DMultiPlanePropagator.h"

Op3DMultiPlanePropagator::Op3DMultiPlanePropagator(Vox _szin, Pix _szout, float _ratio_ext, float _ratio_pad, Vec3d _k_vect, float _n_0, float _z_s, float _NA) 
{
    // // DIMENSIONAL PARAMETERS

    if(_ratio_ext < 1)
        cout << "The extension factor must be >= 1." << endl;

    Size extfovsize(_szout.nb_x * _ratio_ext, _szout.nb_y * _ratio_ext);
    /*
    if ( ~( (szin.nb_y==extfovsize(1)) && (szin.nb_x==extfovsize(2)) ) )
        error('SIZEIN and SIZEOUT are not coherent regarding RATIO_EXT.');
    */
    //cout << "szin.nb_y-extfovsize = " << _szin.nb_y-extfovsize.height << endl;
    //cout << "szin.nb_x-extfovsize = " << _szin.nb_x-extfovsize.width << endl;

    ratio_ext = _ratio_ext;
    flag_ext = ratio_ext>1;
    vox = _szin;
    pix = _szout;

    if( vox.dy!=pix.dy || vox.dx!=pix.dx )
        cout << "Lateral sampling steps (dy,dx) in object and data fields of view must be the same." << endl;

    spl = Vec3d(vox.dx, vox.dy, vox.dz);

    if(_ratio_pad < 1)
        cout << "The padding factor must be >= 1." << endl;
    ratio_pad = _ratio_pad;
    flag_pad = ratio_pad>1;

    Size padfovsize(vox.nb_x * ratio_pad, vox.nb_y * ratio_pad);
    sizepad = Vec2i(padfovsize.width, padfovsize.height);
    pixPadded = { sizepad[0], sizepad[1], pix.dx, pix.dy};

    // // INSTRUMENTAL PARAMETERS

    if(n_0 < 0) 
        cout << "The refractive index of the medium N_0 should be a positive scalar." << endl;
    n_0 = _n_0;

    if(_NA > 1)
        cout << "Numerical aperture NA must be <= 1." << endl;
    NA = _NA;
    if( _z_s<0 ) 
        cout << "z_s must be a positive scalar." << endl;
    z_s = _z_s;

    k_vect = _k_vect;

    k_n_0 = norm(k_vect);
    lambda = 2*PI*n_0/k_n_0;

    // // OPERATOR PARAMETERS
    // // OTHER FLAGS

    // // Calculate plane-to-plane propagator's MTFs

    // Inter-plane propagators
    get_propagator_tilt(Hdz, spl[2], k_vect, n_0, pixPadded, NA);

    // Free propagator to the sensor plane
    Mat z_coord;
    get_fft_compatible_coordinates(z_coord, vox.nb_z, spl[2]);
    virtual_z_s = z_s - z_coord.at<float>(0, vox.nb_z-1);
    get_propagator_tilt(Hzs, virtual_z_s, k_vect, n_0, pixPadded, NA);

    // Calculate illumination waves if storage is required

    // Phase normalization (to have 0 phase on the "sensor" plane)
    Mat temp, cart[2];
    polarToCart(Mat(), -1*k_vect[2], cart[0], cart[1]);
    merge(cart, 2, temp);
    //norm_uinc =  cplxToScalar( polar<float>(-1*k_vect[2]) * (-1*z_coord.at<float>(0,0)+z_s) );
    norm_uinc = polar<float>(1, -1*k_vect[2] * (-1*z_coord.at<float>(0,0)+z_s) );
    //cout << "norm_uinc = " << norm_uinc << endl;
    //cout << "arg norm_uinc = " << arg(norm_uinc) << endl;
    //cout << "abs norm_uinc = " << abs( norm_uinc) << endl;
}
    
void Op3DMultiPlanePropagator::computeProjectionA(Mat& u_tot_a, vector<Mat>& field_u, const vector<Mat>& t) {
    int ny_pad = vox.nb_y*ratio_pad, nx_pad = vox.nb_x*ratio_pad;

    for(int i=0; i<field_u.size(); i++) {
        field_u[i] = Mat(ny_pad, nx_pad, CV_32FC2);
    }
    field_u[0] = Mat::ones(ny_pad, nx_pad, CV_32FC2);

    Mat uk = Mat::ones(ny_pad, nx_pad, CV_32FC2);
    Mat tk;

/*
    imwrite_Complexe("Saved/Hdz", Hdz, C_AMPL|C_PHAS);
    imwrite_Complexe("Saved/Hzs", Hzs, C_AMPL|C_PHAS);
*/

    for(int k=0; k < vox.nb_z-1; k++) {
        tk = t[k];
        
        fov_extension(tk, tk, 2, false ,Scalar(1,0));
        mulSpectrums(field_u[k], tk, uk, 0);
        propagate(field_u[k+1], uk, Hdz, false);

        //waitKey(0);
    }
    
    tk = t[vox.nb_z-1];
    fov_extension(tk, tk, 2, false ,Scalar(1,0) );
    mulSpectrums(field_u[vox.nb_z-2], tk, uk, 0);

    propagate(u_tot_a, uk, Hzs, false);

    // u_tot_a *= norm_uinc;    
    mulSpectrums(u_tot_a, Mat_<Complexe>( u_tot_a.size(),  norm_uinc), u_tot_a, 0);

    fov_extension(u_tot_a, u_tot_a, 2, true);

    
}
