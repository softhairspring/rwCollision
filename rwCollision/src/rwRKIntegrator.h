#ifndef RWRKINTEGRATOR_H
#define RWRKINTEGRATOR_H


#include "rwObjects.h"

struct State
{
	rwVect3 x;      // position
	rwVect3 v;      // velocity
};
struct Derivative
{
	rwVect3 dx;      // dx/dt = velocity
	rwVect3 dv;      // dv/dt = acceleration
};

/***
The Runge–Kutta method
**/
class rwRKIntegrator {


public:
	void integrate(State &state,
		float t,
		float dt)
	{
		Derivative a, b, c, d;

		a = evaluate(state, t, 0.0f, Derivative());
		b = evaluate(state, t, dt*0.5f, a);
		c = evaluate(state, t, dt*0.5f, b);
		d = evaluate(state, t, dt, c);

		rwVect3 dxdt = (a.dx + (b.dx + c.dx)*2.0f + d.dx)* (1.0f / 6.0f);

		rwVect3 dvdt = (a.dv + (b.dv + c.dv)*2.0f + d.dv)* (1.0f / 6.0f);

		state.x = state.x + dxdt * dt;
		state.v = state.v + dvdt * dt;
	}



private:
	Derivative evaluate(const State &initial,
		float t,
		float dt,
		const Derivative &d)
	{
		State state;
		state.x = initial.x + d.dx*dt;
		state.v = initial.v + d.dv*dt;

		Derivative output;
		output.dx = state.v;
		output.dv = acceleration(state, t + dt);
		return output;
	}
	rwVect3 acceleration(const State &state, float t)
	{
		const float k = 10;	//grav
		const float b = 1;	//mass
		return (state.x*-k) - (state.v*b);	//gravity to center 0
	}

};

#endif