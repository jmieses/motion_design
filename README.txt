


The deCasteljau algorithm is used in the Motion file to return a DualQuaternion object, which is
used in the pointMotion method to return a hPoint for drawing. This method is used in the 
main by creating a Motion object and using the Motion m object from the original program. 
In addition, a set of hPoint with the required points are initialized to take advantage of 
the this pointer along with the PointTransform() method.

Second, a DualQuaternion object is form using the given points from the file to obtain a
transformed point via conjugation function. After the dual quaternion is formed from the 
original point, the conjugation formula is used to return a DualQuaternion object. In turn,
the GetDual() method is used to return the dual part of the dual quaternion as a Quaternion 
object. Once the Quaternion object is obtained, it is used in the draw function to draw the 
transformed points. 

Last, different motion designs works properly (homogenous transformation, unit dual quaternion algebra), 
except for the screw motion. Therefore its implementation is commented out, so the program can run properly. 

  