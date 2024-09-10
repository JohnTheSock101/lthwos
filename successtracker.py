#This program just takes text input and stores it nicely in a little
#file with a timestamp.

#use time to keep track of when things happened
import time

#create the success file if it doesn't already exist
try:
	file = open("successes.txt")
except:
	file = open("successes.txt","w")
file.close()


def main(success=""):

	#If success is not empty, don't display anything and just 
	#quietly write it
	if (success):
		file = open("successes.txt","a")
		file.write("On "+time.asctime()+", I "+success+"\n")
		file.close()

	#If success is empty, ask the user for input
	else:
		success = input("What did you accomplish? ")
		file = open("successes.txt","a")
		file.write("On "+time.asctime()+", I "+success+"\n")
		
main()
