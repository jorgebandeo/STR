
float dco 
      (float v,         /* value at one point */
       float w,         /* value at the other point */
       float lambda);   /* contrast parameter */

void diff2d 
     (//float    ht,        /* time step size */
      //float    lambda,    /* contrast parameter */
      long     nx,        /* image dimension in x direction */ 
      long     ny,        /* image dimension in y direction */ 
      float    **f,
      float    lut[]);      /* input: original image ;  output: smoothed */

void performance(); // mede o tempo de execução

