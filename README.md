# network-flux-analysis
graph decomposition / transform

cleaned up code for my dissertation which can be found here: 
  <ul><li>https://getd.libs.uga.edu/pdfs/luper_david_b_201212_phd.pdf</li></ul>
</br></br>
make in the src directory builds an executable called nfa

to execute from the src directory:
  <ul><li>./nfa --directory ../systems/neuse/</li></ul>

</br></br>
the decomposition will produce multiple files in --directory.  the coefficient produced by the decomposition will be in coefficient.vector and they will correspond the fluxes (cycles) in cycles.out.  the coefficients belong to the cycle on the corresponding line number in the cycles file.
