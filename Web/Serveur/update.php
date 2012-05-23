<?php
if (isset($_GET["v"])){
	if (version_compare($_GET["v"], file_get_contents("last_version/version.txt")) != 0){
		echo "/dohome/last_version/dohome.zip";
	} else {
		echo "0";
	}
} else {
	echo "-1";
}
?>