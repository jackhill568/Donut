




float ray_march(vec3 ro, vec3 rd) {
  float d = 0.; // init depth
  for (int i=0; i < MAX_STEPS; ++i) {
    vec3 p = ro + rd * d;
    float ds = SDF(p);
    
    if (ds < SURF_DIST) 
      return d;
      
    d += ds;
  
    if (d > MAX_DIST)
      return 1./0.;
  }
  return 1./0.;
}

int main() {


	return 0;
}
