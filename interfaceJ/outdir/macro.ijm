//InterfaceJ Macro
open("fakeImage.tif");

run("3D object counter...", "threshold=87 slice=33 min.=10 max.=11869184 exclude_objects_on_edges objects surfaces centroids centres_of_masses statistics summary");

saveAs("Results", "Results.txt");
save("output.tif");

