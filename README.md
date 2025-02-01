# cpp-physics-engine
this is gonna be lit

## Gravity

Gravity is simply a constant acceleration $9.8 ms^{-2}$ downwards. 

## Collision Impulse

Impulse is the change in momentum of an object. Since momentum is equal to velocity times weight ($p = mv$), we see that impulse($\Delta p = m \Delta v$) is dependent on the object's velocity and weight. This allows us to determine the result of the collision of objects.

For impulse resolution in our engine, we also consider the coefficient of restitution `e`, which is a measure of the elasticity of a collision between two bodies.

Hence, each object must have the properties:
- Velocity
- Position
- Mass
- Restitution

We also note that if we collide with another object, then <mark>each object recieves the impulse in the direction of the normal to the collision for each object</mark> (eg. a frictionless Tennis ball landing after Federer's serve will experience an impulse directly upwards, and the court will recieve an impulse downwards). Hence we use the magnitute of the impulse in the direction of the normal, so when calculating the impulse we use the velocity in the direction of the normal (by using a **dot product**).

