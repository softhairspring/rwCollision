#ifndef DEFINEDFIGURES_H
#define DEFINEDFIGURES_H

//
//zdefiniowane punkty figur zeby nie robiæ importera z plikow w pocz¹tkowych fazach biblioteki
// koordynaty s¹ podane jako lokalne(relatywne)
//nieu¿ywane poza unit testami
namespace rwFigures {

//sfera r=promien=1 subdivide=1
const int icospheres1r1nr = 36;
double icospheres1r1 [] ;
const int cube2sphere30_sc3_sd3nr = 98*3;
double cube2sphere30_sc3_sd3 [] ;

//
//pary nierozciagniete kolidujace
//

//
//	obiekty rozci¹gniête(extrude)
//

//para 01-02 koliduj¹ca
const int ccd_cube_extr01nr = 198;
double ccd_cube_extr01 [];
const int ccd_cube_extr02nr = 126;
double ccd_cube_extr02 [];
// para 03-04 nie koliduje
const int ccd_cube_extr03nr = 48;
double ccd_cube_extr03 [];
const int ccd_cube_extr04nr = 48;
double ccd_cube_extr04 [];




}//endnamespace

#endif