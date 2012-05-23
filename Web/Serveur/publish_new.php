<?php
if (md5_file("current_version/dohome.zip") != md5_file("last_version/dohome.zip")){
	$zip = new ZipArchive();
	
	$last_version_string = file_get_contents("last_version/version.txt");
	$last_version_array = explode(".", $last_version_string);

	copy("current_version/dohome.zip", "last_version/dohome.zip");

	$zip->open("last_version/dohome.zip");
	$current_version_string = $zip->getFromName("version.txt");
	$current_version_array = explode(".", $current_version_string);
	
	foreach ($last_version_array as $key => $value) {
		if ($current_version_array[$key] != 'x'){
			if ($current_version_array[$key] != $last_version_array[$key]) {
				foreach ($current_version_array as $key2 => $value2) {
					if ($key2 < $key){
						$new_version_array[$key2] = $value2;
					} else {
						$new_version_array[$key2] = "0";
					}
				}
				$new_version_string = implode(".", $new_version_array);
				break;
			}
		} else {
			$new_version_array = $current_version_array;
			$new_version_array = str_replace("x", (string)(strval($last_version_array[sizeof($last_version_array)-1])+1), $new_version_array);
			$new_version_string = implode(".", $new_version_array);
		}
	}

	echo "Version courante $current_version_string <br/>  Derniere version : $last_version_string <br/> Nouvelle version : $new_version_string";
	
	$last_version_file = fopen("last_version/version.txt", "w+");
	
	fputs($last_version_file, $new_version_string);

	$zip->deleteName('version.txt');
	$zip->addFromString('version.txt', $new_version_string);
	
	fclose($last_version_file);
	$zip->close();

	copy("current_version/dohome.zip", "versions/$new_version_string.zip");
} else {
	echo "Pas de Nouvelle version";
}
?>