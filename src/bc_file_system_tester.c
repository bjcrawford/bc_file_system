/**
 * @file bc_file_system_tester.c
 * @author Brett Crawford
 * @brief Constructing a File System and the Related IOCS Functions
 * @details
 *  Course: CIS 3207, Sec 2
 *  Prof: Kwatny
 *  TAs: Liang and Casey
 *  Date: 2014-12-05
 *  Description: This program was written for use in Linux.
*/

#include "bc_file_system.h"

void printBootClusterInfo(FILE**);

int main(int argc, char **argv)
{
	FILE *vd;

	if(!openVirDrive(&vd, "Drive2MB"))
		fprintf(stderr, "Error opening drive");
	fprintf(stdout, "VDrive Opened\n");

	initVirDrive(&vd, "VDrive");

	printBootClusterInfo(&vd);

	BC_FILE *file0 = openFile(&vd, "file0.txt");
	
	createDirectory(&vd, "folder0");

	BC_FILE *subfile0 = openFile(&vd, "folder0/subfile0.txt");

	fprintf(stdout, "Root Directory Listing: \n%s", getDirectoryListing(&vd, "root"));
	fprintf(stdout, "folder0 Directory Listing: \n%s", getDirectoryListing(&vd, "folder0"));
	
	printBootClusterInfo(&vd);

	BC_FILE *brett = openFile(&vd, "one/two/three/brett.txt");

	fprintf(stdout, "Root Directory Listing: \n%s", getDirectoryListing(&vd, "root"));
	fprintf(stdout, "one Directory Listing: \n%s", getDirectoryListing(&vd, "one"));
	fprintf(stdout, "two Directory Listing: \n%s", getDirectoryListing(&vd, "one/two"));
	fprintf(stdout, "three Directory Listing: \n%s", getDirectoryListing(&vd, "one/two/three"));

	char *test69 = "The man in black fled across the desert, and the gunslinger followed.";

	char *test644 = "The man in black fled across the desert and the gunslinger followed. The desert was the apotheosis of all deserts, huge, standing to the sky for what looked like eternity in all directions. It was white and blinding and waterless and without feature save for the faint, cloudy haze of the mountains which sketched themselves on the horizon and the devil-grass which brought sweet dreams, nightmares, death. An occasional tombstone sign pointed the way, for once the drifted track that cut its way through the thick crust of alkali had been a highway. Coaches and buckas had followed it. The world had moved on since then. The world had emptied.";
	
	char *test2066 = "The greatest mystery the universe offers is not life but size. Size encompasses life, and the Tower encompasses size. The child, who is most at home with wonder, says: Daddy, what is above the sky? And the father says: The darkness of space. The child: What is beyond space? The father: The galaxy. The child: Beyond the galaxy? The father: Another galaxy. The child: Beyond the other galaxies? The father: No one knows. You see? Size defeats us. For the fish, the lake in which he lives is the universe. What does the fish think when he is jerked up by the mouth through the silver limits of existence and into a new universe where the air drowns him and the light is blue madness? Where huge bipeds with no gills stuff it into a suffocating box and cover it with wet weeds to die? Or one might take the tip of the pencil and magnify it. One reaches the point where a stunning realization strikes home: The pencil tip is not solid; it is composed of atoms which whirl and revolve like a trillion demon planets. What seems solid to us is actually only a loose net held together by gravity. Viewed at their actual size, the distances between these atoms might become league, gulfs, aeons. The atoms themselves are composed of nuclei and revolving protons and electrons. One may step down further to subatomic particles. And then to what? Tachyons? Nothing? Of course not. Everything in the universe denies nothing; to suggest an ending is the one absurdity. If you fell outward to the limit of the universe, would you find a board fence and signs reading DEAD END? No. You might find something hard and rounded, as the chick must see the egg from the inside. And if you should peck through the shell (or find a door), what great and torrential light might shine through your opening at the end of space? Might you look through and discover our entire universe is but part of one atom on a blade of grass? Might you be forced to think that by burning a twig you incinerate an eternity of eternities? That existence rises not to one infinite but to an infinity of them?";

	writeFile(&vd, test69, 69, brett);
	writeFile(&vd, test644, 644, file0);
	writeFile(&vd, test2066, 2066, subfile0);

	bc_rewind(brett);
	char *strRead1 = calloc(70, sizeof(char));
	readFile(&vd, strRead1, 0, 69, brett);
	printf("%s\n", strRead1);

	bc_rewind(file0);
	char *strRead2 = calloc(645, sizeof(char));
	readFile(&vd, strRead2, 0, 644, file0);
	printf("%s\n", strRead2);

	bc_rewind(subfile0);
	char *strRead3 = calloc(2067, sizeof(char));
	readFile(&vd, strRead3, 0, 2066, subfile0);
	printf("%s\n", strRead3);

	printBootClusterInfo(&vd);

	closeVirDrive(&vd);

    return 0;
}

void printBootClusterInfo(FILE **virDrive)
{
	fprintf(stdout, "\n===================================================\n");
	fprintf(stdout, "Initialized: %zu\n", getInitialized(virDrive));
	fprintf(stdout, "Drive Label: %s\n", getDriveLabel(virDrive));
	fprintf(stdout, "Bytes Per Cluster: %zu\n", getBytesPerCluster(virDrive));
	fprintf(stdout, "Number Of Reserved Clusters: %zu\n", getNumberOfReservedClusters(virDrive));
	fprintf(stdout, "Number Of Clusters On Drive: %zu\n", getNumberOfClustersOnDrive(virDrive));
	fprintf(stdout, "Number Of Clusters Per FAT: %zu\n", getNumberOfClustersPerFAT(virDrive));
	fprintf(stdout, "First Cluster Of Root Dir: %zu\n", getFirstClusterOfRootDir(virDrive));
	fprintf(stdout, "Number Of Free Clusters: %zu\n", getNumberOfFreeClusters(virDrive));
	fprintf(stdout, "Next Free Cluster: %zu\n", getNextFreeCluster(virDrive));
	fprintf(stdout, "Size Of Drive: %zu\n", getSizeOfDrive(virDrive));
	fprintf(stdout, "===================================================\n\n");
}
