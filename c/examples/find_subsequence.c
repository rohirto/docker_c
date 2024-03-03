/**
 * @file find_subsequence.c
 * @author rohirto
 * @brief Find the no of matching characters in 2 strings
 * @version 0.1
 * @date 2023-12-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * Suppose String 1 is AquaVitae
 *         String 2 is AruTaVae
 * Give me the no of characters same to both string, ie Longest  Common Sequence LCS = 6 (6 letters coomon to them)
 * 
 * Explanation:

Initialization:
n and m are the lengths of the strings 
dp is a 2D  where dp[i][j] represents the length of the LCS for the prefixes of str1 and str2 of lengths i and j respectively.
The array is initialized with zeros.

Dynamic Programming Loop:
The nested loops iterate through all possible pairs of prefixes of str1 and str2.
For each pair (i, j), the algorithm considers the characters str1[i-1] and str2[j-1] (since indices start from 0).
If the characters match, it increments the length of the LCS by 1 (dp[i][j] = dp[i-1][j-1] + 1).
If the characters don't match, it chooses the maximum length from either the top or left cell (dp[i][j] = max(dp[i-1][j], dp[i][j-1])).
Result:

The final result is stored in dp[n][m], representing the length of the LCS for the entire strings str1 and str2.
This value is then printed.
 * 
 */

#include <stdio.h>
#include <string.h>


void init_2d_array(int m, int n, int arr[][n])
{
    int i, j;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
        arr[i][j] = 0;
}

int max(int a, int b)
{
    if(a < b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

int find_LCS(char* str1, char* str2)
{
    int n = strlen(str1);
    int m = strlen(str2);

    int dp[n+1][m+1];

    init_2d_array(n,m,dp);  //initialize array with zero

    for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                if (str1[i - 1] == str2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        return dp[n][m];
}

int main()
{
    char s1[20] = "AquaVitae";
    char s2[20] = "AruTaVae";

    int LCS = find_LCS(s1,s2);

    printf("LCS of %s and %s is %d", s1,s2,LCS);

    return 0;
}