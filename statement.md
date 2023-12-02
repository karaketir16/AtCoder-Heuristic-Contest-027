### Story

F Corporation developed a robotic vacuum cleaner, Takahashi-kun cleaner No.2, and decided to entrust it with the cleaning of their office. Takahashi-kun cleaner No.2 can operate indefinitely through solar power and repeats cleaning on a predetermined route indefinitely. The office has varying levels of susceptibility to dirt in different areas, and by frequently cleaning the areas that are more prone to dirt, the entire office can be kept cleaner.

### Problem Statement

There is an \(N \times N\) square board. Let (0, 0) be the coordinates of the top-left square, and \( (i, j) \) be the coordinates of the square located \( i \) squares down and \( j \) squares to the right from there. The perimeter of the \( N \times N \) board is surrounded by walls, and there may also be walls between adjacent squares.

Each square \( (i, j) \) is assigned a value \( d_{i, j} \) which represents its susceptibility to dirt. Your task is to clean these squares by moving around the board. You can move to an adjacent square that is not blocked by a wall. After the move, the dirtiness of the square you moved to becomes 0, and the dirtiness of all other squares \( (i, j) \) increases by \( d_{i, j} \). Consider a cleaning route that starts and ends at (0, 0), with a length (number of moves) not exceeding \( 10^5 \). The cleaning route may pass through the same square multiple times, but must visit each square at least once.

Let \( a_{t,i,j} \) denote the dirtiness of each square \( (i, j) \) after the \( t \)-th move, and let \( S_t = \sum_{i=0}^{N-1}\sum_{j=0}^{N-1} a_{t,i,j} \) denote the total dirtiness. At \( t = 0 \), we assume that the dirtiness of all squares is \( a_{0,i,j} = 0 \). Define the **average dirtiness** as \[ \bar{S} = \frac{1}{L} \sum_{t=L}^{2L-1}S_t, \] which is the average of the total dirtiness during the period \( t = L, L+1, \cdots, 2L-1 \) when the cleaning route of length \( L \) is repeated infinitely.

Please find a cleaning route that minimizes the average dirtiness as much as possible.

#### The Meaning of Average Dirtiness

We can prove that \( a_{t,i,j} = a_{t+L,i,j} \) for \( t \geq L \) when the cleaning route of length \( L \) is repeated infinitely. Therefore, considering the average \[ \frac{1}{T} \sum_{t=0}^{T-1} S_t \] of the total dirtiness up to \( T \) turns, its limit as \( T \to \infty \) coincides with the average dirtiness.

### Scoring

Let \(\bar{S}\) be the average dirtiness of the output cleaning route. Then you will obtain an absolute score of \(\mathrm{round}(\bar{S})\). The lower the absolute score, the better. If you output an illegal cleaning route (length exceeds \(10^5\), does not return to \( (0,0) \), there is an unvisited square, or it hits a wall), it will be judged as WA.

For each test case, we compute the **relative score** \(\mathrm{round}(10^9 \times \frac{\mathrm{MIN}}{\mathrm{YOUR}})\), where YOUR is your absolute score and MIN is the lowest absolute score among all competitors obtained on that test case. The score of the submission is the sum of the relative scores.

The final ranking will be determined by the system test with more inputs which will be run after the contest is over. In both the provisional/system test, if your submission produces illegal output or exceeds the time limit for some test cases, only the score for those test cases will be zero, and your submission will be excluded from the MIN calculation for those test cases.

The system test will be performed only for **the last submission which received a result other than CE**. Be careful not to make a mistake in the final submission.

#### Number of test cases
- Provisional test: 50
- System test: 2000. We will publish [seeds.txt](https://img.atcoder.jp/ahc027/seeds.txt) (sha256=cdea33a6050850bf1387e2191b802a1df7e43fcb969fd6c3bf9cbd96a4d790d7) after the contest is over.

#### About relative evaluation system
In both the provisional/system test, the standings will be calculated using only the last submission which received a result other than CE. Only the last submissions are used to calculate the MIN for each test case when calculating the relative scores.

The scores shown in the standings are relative, and whenever a new submission arrives, all relative scores are recalculated. On the other hand, the score for each submission shown on the submissions page is the sum of the absolute score for each test case, and the relative scores are not shown. In order to know the relative score of submission other than the latest one in the current standings, you need to resubmit it. If your submission produces illegal output or exceeds the time limit for some test cases, the score shown on the submissions page will be 0, but the standings show the sum of the relative scores for the test cases that were answered correctly.

#### About execution time
Execution time may vary slightly from run to run. In addition, since system tests simultaneously perform a large number of executions, it has been observed that execution time increases by several percent compared to provisional tests. For these reasons, submissions that are very close to the time limit may result in TLE in the system test. Please measure the execution time in your program to terminate the process, or have enough margin in the execution time.