#include <iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define antNum 4

using namespace std;

int main()
{
    int n;
    cout << "Enter the problem size." << endl;
    cin >> n;
    int weight[n],value[n];
    for (int i=0;i<n;i++)
    {
        cout << "Enter the weight of " << i+1 << " item.\n";
        cin >> weight[i];
        cout << "Enter the value of " << i+1 << " item.\n";
        cin >> value[i];
    }
    int sackSize;
    cout << "Enter the knapsack limit.\n";
    cin >> sackSize;

    float tau[n],eta[n],Dtau[n]; //remaining pheromone and inspiring factor
    float alpha=0.7,beta=2.3;
    for(int i=0;i<n;i++)
    {
        tau[i]=1;
        eta[i]=(float)value[i]/(float)weight[i];
    }

    int nc=1;//loop times
    int tabu[antNum][n],Move[antNum][n];
    float probability[antNum][n];
    int curr_weight[antNum],curr_value[antNum];
    int bestSolution=0;

    for(int i=0;i<antNum;i++)
    {
        for(int j=0;j<n;j++)
            {
                tabu[i][j]=0;
                probability[i][j]=0;
            }
    }

    for (int i=0;i<antNum;i++)
    {
        Move[i][i]=1; //every ant on the first items initially
        tabu[i][i]=1;
        curr_value[i]=value[i];
        curr_weight[i] = sackSize-weight[i];
    }
    while(nc>0)
    {
        for (int i=0;i<antNum;i++) //calculate moving probability
        {
            float sum=0;
            for(int j=0;j<n;j++)
                if (tabu[i][j]!=1)
                    sum+=pow(tau[j],alpha)*pow(eta[j],beta);
            for(int j=0;j<n;j++)
                {
                    if (tabu[i][j]==1)
                        probability[i][j]=0;
                    else
                        probability[i][j]= pow(tau[j],alpha)*pow(eta[j],beta)/sum;
                }
        }

        for(int i=0;i<antNum;i++) //moving to new points
        {
            for(int j=0;j<n;j++)
            {
                float rann;
                srand(time(NULL));
                rann = (rand()%100)*0.01/antNum;
                if (probability[i][j]>rann && curr_weight[i]>=weight[j])
                {
                    Move[i][j]=1;
                    tabu[i][j]=1;
                    curr_weight[i]-=weight[j];
                    curr_value[i]+=value[j];
                    break;
                }
            }
        }

        for (int i=0;i<antNum;i++) //updating best solution
            if (curr_value[i]>bestSolution)
                bestSolution=curr_value[i];

        float Q=1;// calculating delta tau
        for (int i=0;i<antNum;i++)
        {
            for (int j=0;j<n;j++)
            {
                if (Move[i][j]==1)
                {
                    Dtau[j]=Q/curr_weight[i];
                    Move[i][j]=0;
                    break;
                }
            }
        }

        float rho=0.5;//updating tau
        for(int i=0;i<n;i++)
            tau[i] = rho*tau[i]+Dtau[i];
        for(int i=0;i<n;i++)
            Dtau[i]=0;

        nc--;
    }

    cout << bestSolution << endl;
    return 0;
}
