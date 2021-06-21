# NuGenTKI
  ## Documentation of source codes
   http://www-pnp.physics.ox.ac.uk/~luxi/transport/doxygen/NuGenTKI

  ## Quick how-to
First checkout TKI package in some location $TKI_PARENT
  ```
  cd $TKI_PARENT
  git clone https://github.com/luxianguo/TKI.git
  ```
Then checkout NuGenTKI package in some other location $NUGENTKI_PARENT
  ```
  cd $NUGENTKI_PARENT
  git clone https://github.com/luxianguo/NuGenTKI.git
  ```
style/, bin/, and include/AnaFunctions.h are from TKI. Need to link it to NuGenTKI/
   ```
  cd $NUGENTKI_PARENT/NuGenTKI
  ln -s $TKI_PARENT/TKI
   ```
