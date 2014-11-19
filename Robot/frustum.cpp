//Based on the files in lightouse
#include "frustum.h"
#include <math.h>
#include "Vector3.h"
#include "camera.h"
#include <iostream>
#include "node.h"



Frustum::Frustum() {}

void Frustum::setCamInternals(double angle, double ratio, double nearD, double farD) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	nh = nearD * tan(angle*M_PI/180*0.5);
	nw = nh * ratio;
	fh = farD  * tan(angle*M_PI / 180 * 0.5);
	fw = fh * ratio;


}


void Frustum::setCamDef(Vector3 p, Vector3 l, Vector3 u) {

	Vector3 dir, nc, fc, x,y,z;
	z = p - l;
	z.normalize();

	x = x.cross(u, z);
	x.normalize();

	y = y.cross(z,x);
	y.normalize(); 

	nc = p - z.altScale(nearD);
	fc = p - z.altScale(farD);
	ntl = nc + y.altScale(nh);
	ntl = ntl - x.altScale(nw);
	ntr = nc + y.altScale(nh);
	ntr = ntr + x.altScale(nw);
	nbl = nc - y.altScale(nh);
	nbl = nbl - x.altScale(nw);
	nbr = nc - y.altScale(nh);
	nbr = nbr + x.altScale(nw);
	ftl = fc + y.altScale(fh) ;
	ftl = ftl - x.altScale(fw);
	ftr = fc + y.altScale(fh);
	ftr = ftr + x.altScale(fw);
	fbl = fc - y.altScale(fh);
	fbl = fbl - x.altScale(fw);
	fbr = fc - y.altScale(fh) ;
	fbr = fbr + x.altScale(fw);
	//ntl.print("ntl");
	//ntr.print("ntr");
	//nbr.print("nbr");
	//nbl.print("nbl");
	//ftl.print("ftl");
	//ftr.print("ftr");
	//fbr.print("fbr");
	//fbl.print("fbl");
	pl[0].set3Points(ntr, ntl, ftl);
	//pl[0].print();
	//pl[0].point.print("A");
	pl[1].set3Points(nbl, nbr, fbr);
	//pl[1].print();
	//pl[1].point.print("A");
	pl[2].set3Points(ntl, nbl, fbl);
	//pl[2].print();
	//pl[2].point.print("A");
	pl[3].set3Points(nbr, ntr, fbr);
	//pl[3].print();
	//pl[3].point.print("A");
	pl[4].set3Points(ntl, ntr, nbr);
	//pl[4].print();
	//pl[4].point.print("A");
	pl[5].set3Points(ftr, ftl, fbl);
	//pl[5].print();
	//pl[5].point.print("A");
}

int Frustum::sphereInFrustum(Vector4 p, double radius) {
	Vector3 temp;
	temp.set(p.get(0), p.get(1), p.get(2));
	int result = 1;
	double distance;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(temp);
		if (distance < -radius) {
			//std::cout << "Radius " << radius << std::endl;
			//std::cout << distance << std::endl;
			return -1;
			}
		else if (distance < radius) {
			result = 0;
		}
	}
	return(result);

}


