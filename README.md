# cs201project_group19

The project is a simple implementation of a voting system based on a merkle tree.

It is implemented as 
          #the code votinggram takes a uniqueid as input and creates a hash using the sha256 algorithm
          #the voting hash of each voter is stored in a file specific to the party.
          ->
          votinggram.c along with sha256.c is used for this
          A merkle tree is used to audit the whole process.
          The no of votes for each party is determined by no of hashes.


          Any changes can easily be found because the merkleroot changes.
          The whole merkle tree and its corresponding nodes are public and any one a check if the process going on
          is correct.
          votingcount.c is along with sha256.c is used to count the number of votes and determine the winner.


          

          
          
