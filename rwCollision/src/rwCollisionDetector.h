#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "rwObjects.h"

/*

Klasa prototypowa szukaj¹ca kolizji miedzy obiektami
algorytmy wykrywania kolizji znajduja sie w innych klassach
Klasa (nie)powinna wykonac tylko przesuniecie obiektu do momentu kolizji
dalsza zmiana wektorów ruchu/kierunków i nastepstwa kolizji powinny byæ zmienione
w pêtli g³ównej/specjalnej klasie do tego.
**/
class rwCollisionDetector {

protected:

	ObjectsList Objects;
	float timestep;

	///minimalny stosunek przebytej drogi w sekundzie do wielkoœci figury,
	///potrzebny aby zastosowac extrapolacje obiektu
	float minimum_sweep_for_extrude;
	///czy uzywamy CCD - zmiana wartosci powinna byc implementowana przez inne klasy
	///zaleznie od tego czy obsluguja CCD
	bool ISCCD;	
	bool ISBROAD;
public:

	rwCollisionDetector(){
		minimum_sweep_for_extrude = 0.25;
		ISCCD=false;
		ISBROAD=true;
	}
	/**
	 wykrycie wstepnych kolizji BBox'ów
	 z uzyciem extrude dla szybkich obiektow
	 **/
	virtual bool solveBroad(){
		return false;
	}
	/**
	wykrycie dokladnej kolizji obiektów które zosta³y wykryte w fazie broad
	jako koliduj¹ce
	**/
	virtual bool solveNarrow(){
		return false;
	}


	void updateObjectsList(ObjectsList & list, float time_step){
		Objects = list;
		timestep = time_step;
	}

	virtual void setCCD(bool ccd)	{ISCCD = ccd;}
	virtual bool isCCD()			{ return ISCCD;}
	virtual void setBroadPhase(bool broad)	{ISBROAD= broad;}
	virtual bool isBroadPhase()			{ return ISBROAD;}

};




#endif