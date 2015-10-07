cp "Audible Altimeter.kicad_pcb" "Audible Altimeter_old.kicad_pcb"
sed -r '/fp_text/N;s/(\(fp_text.+\n.*\(size )[^)]+\) \(thickness [^)]+/\10.762 0.762) (thickness 0.130/' "Audible Altimeter_old.kicad_pcb" > "Audible Altimeter.kicad_pcb"
