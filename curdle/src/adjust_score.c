

/** \file adjust_score.c
 * \brief Functions for safely amending a player's score in the
 * `/var/lib/curdle/scores` file.
 *
 * Contains the \ref adjust_score_file function, plus supporting data
 * structures and functions used by that function.
 *
 * ### Known bugs
 *
 * \bug The \ref adjust_score_file function does not yet work.
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// for off_t
#include <sys/types.h>


/** Adjust the score for player `player_name`, incrementing it by
  * `score_to_add`. The player's current score (if any) and new score
  * are stored in the scores file at `/var/lib/curdle/scores`.
  * The scores file is owned by user ID `uid`, and the process should
  * use that effective user ID when reading and writing the file.
  * If the score was changed successfully, the function returns 1; if
  * not, it returns 0, and sets `*message` to the address of
  * a string containing an error message. It is the caller's responsibility
  * to free `*message` after use.
  *
  * \todo implement the function.
  *
  * \param uid user ID of the owner of the scores file.
  * \param player_name name of the player whose score should be incremented.
  * \param score_to_add amount by which to increment the score.
  * \param message address of a pointer in which an error message can be stored.
  * \return 1 if the score was successfully changed, 0 if not.
  */
int adjust_score(uid_t uid, const char * player_name, int score_to_add, char **message) 
{

  FILE* fd;
  int buffer;
  int score = 0;
  long curr_pos = 0l;
  char * restrict errorMsg = NULL;
  char * restrict newErrorMSG = NULL;
  char lines[21] = {0};
  char* newline = NULL;
  uid_t currentUser = 0;
  size_t end_of_line;
  int updateScore = 0;
  int end_search = 0;


  //    Check parameters
  if(player_name == NULL || message == NULL || score_to_add == 0)
  {
    if(geteuid() != 0)
    {
      //    tell to run as root?
      errorMsg = "try running as root";
    }
    return 0;
  }
  //    If parameters are valid 
  else if(player_name != NULL && message != NULL || score_to_add >= 0)
  {
    //  get the current user
    currentUser = geteuid();
    seteuid(uid);
    //  Open the file in restrict read mode
    fd = fopen("/var/lib/curdle/scores", "r+"); 
    
    if(fd == NULL)
    {
        printf("Value of errno: %d",errno);
        printf("\nError Message: %s",strerror(errno));
        perror("Message from perror");
    }
    else
    {
      //    While the end of the file hasn't been reached
      while(feof(fd) == 0)
      {
        //  get current position in the file
        curr_pos = ftell(fd);
        end_of_line  = sizeof(lines)/sizeof(lines[0]);
        memset(lines,0, end_of_line);
        buffer = fread(lines,1,end_of_line,fd);

        if(buffer)
        {
          errorMsg = "Try again";
          break;
        }
        //  if we reach \n that means we are at the end of the
        //  line
        else if(lines[end_of_line - 1] == '\n' && end_of_line == buffer)
        {
          score = atoi(lines + 10);
          updateScore = score + score_to_add;

          // buffer overflow
          if(score_to_add < updateScore || score > updateScore)
          {
            //send error msg
            errorMsg = "overflow";
          }
          else{
            fseek(fd, curr_pos, SEEK_SET);
          }
          end_search = 1;
          break;
        }
      }

      //    reached end of file
      //    write to the file and close it
      //    allocate memory for an error message using malloc() 
      //    write an error message to that memory, and 
      //    set *message to the newly allocated memory.
      if(errorMsg)
      {
        newErrorMSG = (char*) malloc(strlen(errorMsg)+1);
        if(newErrorMSG)
        {
          strcpy(newErrorMSG, errorMsg);
          *message = newErrorMSG;
        }
      }
      else
      {
        memset(lines, 0, end_of_line);
        //  The snprintf() function accepts an argument ‘n’, which indicates the maximum number of characters 
        //  (including at the end of null character) to be written to buffer. 
        snprintf(lines, 10, "%s", player_name);
        //snprintf(lines+10,11,"%d",(1==end_search)?updateScore:score_to_add);
        
        
        snprintf(lines + 10, 11, "%d",score_to_add);
        lines[end_of_line-1] = '\n';
        fwrite (lines,1,end_of_line,fd);
      }
    }
    //  close reading of file
    fclose(fd);
    //  set fd to NULL
    fd = NULL;
    //  set current user to default, 0
    seteuid(currentUser);
    return 1;
  }
  return 0;
}