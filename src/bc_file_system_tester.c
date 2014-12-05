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

/* Used for toggling the pause between tests */
#define PAUSE 1

void testRun1();
void testRun2();
void printBootClusterInfo();
void printDirectoryListing(char *dir);
void pause(int pause);

int main(int argc, char **argv)
{
	int input = 0;

	fprintf(stdout, "Constructing a file system\n");
	fprintf(stdout, "Brett Crawford\n\n");
	
	fprintf(stdout, "This program will test the file system I have created.\n\n");

	fprintf(stdout, "Test run 1 will initialize the unused Drive2MB virtual drive and\n");
	fprintf(stdout, "perform several file creations, directory creations, file reads,\n");
	fprintf(stdout, "and file writes.\n\n");

	fprintf(stdout, "Test run 2 will reopen the previously initialized Drive2MB and\n");
	fprintf(stdout, "display the boot record and the contents on the virtual drive.\n\n");

	fprintf(stdout, "Note: Test run 1 should be performed before test run 2.\n\n");

	while(input != 1 && input != 2)
	{
		fprintf(stdout, "Please choose the test to perform (1 or 2): ");
		scanf("%d", &input);
	}

	if(input == 1)
		testRun1();
	else
		testRun2();

	fprintf(stdout, "\nTest finished. Exiting program.\n");

	return 0;
}

void testRun1()
{
	pause(PAUSE);

	initFileSystem("Drive2MB", "2MB_VDrive");

	if(!virDrive)
	{
		fprintf(stderr, "Error opening drive. Exiting");
		exit(1);
	}
	fprintf(stdout, "Virtual drive opened\n");

	printBootClusterInfo();

	pause(PAUSE);

	fprintf(stdout, "Opening 'testfile0.txt'\n");
	BC_FILE *testfile0 = openFile("testfile0.txt");
	
	fprintf(stdout, "Creating 'directory0'\n");
	createDirectory("directory0");

	fprintf(stdout, "Opening 'directory0/testfile1.txt'\n");
	BC_FILE *testfile1 = openFile("directory0/testfile1.txt");

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");

	printBootClusterInfo();

	pause(PAUSE);

	fprintf(stdout, "Opening 'directory1/directory2/directory3/testfile2.txt'\n");
	BC_FILE *testfile2 = openFile("directory1/directory2/directory3/testfile2.txt");

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");
	printDirectoryListing("directory1");
	printDirectoryListing("directory1/directory2");
	printDirectoryListing("directory1/directory2/directory3");

	pause(PAUSE);

	fprintf(stdout, "Writing 69 characters to 'directory1/directory2/directory3/testfile2.txt'\n");
	char *test69 = "The man in black fled across the desert, and the gunslinger followed.";
	writeFile(test69, 69, testfile2);

	fprintf(stdout, "Writing 645 characters to 'testfile0.txt'\n");
	char *test644 = "The man in black fled across the desert, and the gunslinger followed.\nThe desert was the apotheosis of all deserts, huge, standing to the sky\nfor what looked like eternity in all directions. It was white and blinding\nand waterless and without feature save for the faint, cloudy haze of the\nmountains which sketched themselves on the horizon and the devil-grass which\nbrought sweet dreams, nightmares, death. An occasional tombstone sign pointed\nthe way, for once the drifted track that cut its way through the thick crust\nof alkali had been a highway. Coaches and buckas had followed it. The world\nhad moved on since then. The world had emptied.";
	writeFile(test644, 645, testfile0);

	fprintf(stdout, "Writing 2066 characters to 'directory0/testfile1.txt'\n");
	char *test2066 = "The greatest mystery the universe offers is not life but size. Size\nencompasses life, and the Tower encompasses size. The child, who is\nmost at home with wonder, says: Daddy, what is above the sky? And the\nfather says: The darkness of space. The child: What is beyond space?\nThe father: The galaxy. The child: Beyond the galaxy? The father:\nAnother galaxy. The child: Beyond the other galaxies? The father: No\none knows. You see? Size defeats us. For the fish, the lake in which\nhe lives is the universe. What does the fish think when he is jerked\nup by the mouth through the silver limits of existence and into a new\nuniverse where the air drowns him and the light is blue madness? Where\nhuge bipeds with no gills stuff it into a suffocating box and cover it\nwith wet weeds to die? Or one might take the tip of the pencil and\nmagnify it. One reaches the point where a stunning realization strikes\nhome: The pencil tip is not solid; it is composed of atoms which whirl\nand revolve like a trillion demon planets. What seems solid to us is\nactually only a loose net held together by gravity. Viewed at their\nactual size, the distances between these atoms might become league,\ngulfs, aeons. The atoms themselves are composed of nuclei and revolving\nprotons and electrons. One may step down further to subatomic\nparticles. And then to what? Tachyons? Nothing? Of course not.\nEverything in the universe denies nothing; to suggest an ending is the\none absurdity. If you fell outward to the limit of the universe, would\nyou find a board fence and signs reading DEAD END? No. You might find\nsomething hard and rounded, as the chick must see the egg from the\ninside. And if you should peck through the shell (or find a door), what\ngreat and torrential light might shine through your opening at the end\nof space? Might you look through and discover our entire universe is\nbut part of one atom on a blade of grass? Might you be forced to think\nthat by burning a twig you incinerate an eternity of eternities? That\nexistence rises not to one infinite but to an infinity of them?";
	writeFile(test2066, 2066, testfile1);

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");
	printDirectoryListing("directory1");
	printDirectoryListing("directory1/directory2");
	printDirectoryListing("directory1/directory2/directory3");

	pause(PAUSE);

	fprintf(stdout, "Rewinding and reading 69 characters from 'directory1/directory2/directory3/testfile2.txt':\n\n");
	rewindBC_File(testfile2);
	char strRead2[70];
	readFile(strRead2, 69, testfile2);
	strRead2[69] = '\0';
	printf("%s\n\n", strRead2);

	fprintf(stdout, "Rewinding and reading 645 characters from 'testfile0.txt':\n\n");
	rewindBC_File(testfile0);
	char strRead0[646];
	readFile(strRead0, 645, testfile0);
	strRead0[645] = '\0';
	printf("%s\n\n", strRead0);

	fprintf(stdout, "Rewinding and reading 2066 characters from 'directory0/testfile1.txt':\n\n");
	rewindBC_File(testfile1);
	char strRead1[2067];
	readFile(strRead1, 2066, testfile1);
	strRead1[2066] = '\0';
	printf("%s\n\n", strRead1);

	printBootClusterInfo();

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");
	printDirectoryListing("directory1");
	printDirectoryListing("directory1/directory2");
	printDirectoryListing("directory1/directory2/directory3");

	pause(PAUSE);

	fprintf(stdout, "Attempting to print non-existent directories\n");

	fprintf(stdout, "\n");
	printDirectoryListing("directory1/directory4/directory3");
	printDirectoryListing("directory1/directory2/directory3/directory4");

	pause(PAUSE);

	fprintf(stdout, "Deleting 'testfile0.txt'\n");
	deleteFile(testfile0);

	printBootClusterInfo();

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");
	printDirectoryListing("directory1");
	printDirectoryListing("directory1/directory2");
	printDirectoryListing("directory1/directory2/directory3");

	pause(PAUSE);

	fprintf(stdout, "Closing 'testfile1.txt'\n");
	closeFile(testfile1);
	fprintf(stdout, "Closing 'testfile2.txt'\n");
	closeFile(testfile2);

	closeFileSystem();
}

void testRun2()
{
	pause(PAUSE);

	initFileSystem("Drive2MB", "");

	if(!virDrive)
	{
		fprintf(stderr, "Error opening drive. Exiting");
		exit(1);
	}
	fprintf(stdout, "Virtual drive opened\n");

	printBootClusterInfo();

	pause(PAUSE);

	fprintf(stdout, "\n");
	printDirectoryListing("root");
	printDirectoryListing("directory0");
	printDirectoryListing("directory1");
	printDirectoryListing("directory1/directory2");
	printDirectoryListing("directory1/directory2/directory3");

	pause(PAUSE);
}

void printBootClusterInfo()
{
	fprintf(stdout, "\nBoot Cluster Info:\n\n");
	fprintf(stdout, "    Parameter                   |        Value       \n");
	fprintf(stdout, "  ===================================================\n");
	fprintf(stdout, "    Initialized                 | %17u\n", bootRecord->init);
	fprintf(stdout, "    Drive Label                 | %17s\n", bootRecord->label);
	fprintf(stdout, "    Bytes Per Cluster           | %17u\n", bootRecord->bytesPerCluster);
	fprintf(stdout, "    Number Of Reserved Clusters | %17u\n", bootRecord->reservedClusters);
	fprintf(stdout, "    Number Of Clusters On Drive | %17u\n", bootRecord->clustersOnDrive);
	fprintf(stdout, "    Number Of Clusters Per FAT  | %17u\n", bootRecord->clustersPerFat);
	fprintf(stdout, "    First Cluster Of Root Dir   | %17u\n", bootRecord->rootDirStart);
	fprintf(stdout, "    Number Of Free Clusters     | %17u\n", bootRecord->freeClusters);
	fprintf(stdout, "    Next Free Cluster           | %17u\n", bootRecord->nextFreeCluster);
	fprintf(stdout, "    Size Of Drive               | %17u\n", bootRecord->driveSize);
	fprintf(stdout, "  ===================================================\n\n");
}

void printDirectoryListing(char *dir)
{
	char *listing = getDirectoryListing(dir);
	fprintf(stdout, "'%s/' directory listing: \n\n%s\n", dir, listing);
	free(listing);
}

void pause(int pause)
{
	if(pause)
	{
		fprintf(stdout, "Press enter to continue\n");
		getchar();
	}
}
