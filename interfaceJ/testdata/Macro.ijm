//InterfaceJ Macro
//print("123");

open("topro.tif");
setAutoThreshold("Default");
//run("Threshold...");
setThreshold(7, 70);
setOption("BlackBackground", false);
run("Convert to Mask", "method=Default background=Light");
run("Analyze Particles...", "size=4.63-Infinity circularity=0.00-1.00 show=Nothing display clear stack");
saveAs("Results", "Results.xls");
save("d.tif");

run("Quit");
