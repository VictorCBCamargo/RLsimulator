#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iomanip>

using namespace std;

void sorting( float *numbers, int tam)
{
    for(int i = 0; i < tam - 1; i++)
    {
        float currentMin = numbers[i];
        int currentMinIndex = i;

        for(int j = i + 1; j < tam; j++)
        {
            if(currentMin > numbers[j])
            {
                currentMin = numbers[j];
                currentMinIndex = j;
            }
        }

        if(currentMinIndex != i)
        {
            numbers[currentMinIndex] = numbers[i];
            numbers[i] = currentMin;

        }
    }
}

float minArray( float *x, int tam)
{
    if (tam <= 0) return 0.0;

    int minPositive = x[0];

    for( int i = 1; i < tam; i++ )
    {
        if( minPositive > x[i] )
            minPositive = x[i];
    }
    return minPositive;
}

float maxArray( float *x, int tam )
{
    if (tam <= 0) return 0.0;

    int maxPositive = x[0];

    for( int i = 1; i < tam; i++ )
    {
        if( maxPositive < x[i] )
            maxPositive = x[i];
    }
    return maxPositive;
}

int SumI( int * v, int tam )
{      int sum=0;
       for( int i=0; i< tam;i++)
               sum+=v[i];
       return sum;
}

double SumD( double * v, int tam )
{      double sum=0;
       for(int i=0;i<tam;i++)
               sum+=v[i];
       return sum;
}

double AverageD( double * v, int tam )
{      double sum = SumD( v, tam );
       return sum/tam;
}

double DeviationD( double * v, int tam, double ave)
{
    double E=0;

    double inverse = 1.0 / ((double)tam - 1);
    for(unsigned int i=0;i<tam;i++)
    {
        E += pow( ((double)v[i]) - ave, 2);
    }
    return sqrt(inverse * E);
}

double RangeD( double * v, int tam )
{
    double maximo, minimo;

    maximo = v[0];
    minimo = v[0];
    for( int i = 1; i < tam; i++ )
    {
        if ( v[i] > maximo )
        {
            maximo = v[i];
        }
        if ( v[i] < minimo )
        {
            minimo = v[i];
        }
    }

    return (maximo - minimo);
}

double VarianceD( double * v, int tam, double ave)
{
    double E=0;

    for(unsigned int i=0;i<tam;i++)
    {
        E += pow(((double)v[i]) - ave, 2);
    }
    return E/(tam-1);
}

int main()
{
    cout.setf(ios::fixed);
    cout.precision(3);

    char str [10];
    FILE * pFile;

    int sde;
    int jj;
    int kk;
    int mm;
    int nn;

    pFile = fopen ("input.txt","r");
    fscanf (pFile, "%s", str);
    fscanf (pFile, "%d", &sde);
    fscanf (pFile, "%s", str);
    fscanf (pFile, "%d", &jj);
    fscanf (pFile, "%s", str);
    fscanf (pFile, "%d", &kk);
    fclose (pFile);

    int cjto_M[] = { 20, 50, 100, 200, 500 };
    int cjto_N[] = { 5, 10 };

    int N = (int)( sizeof(cjto_N) / sizeof(cjto_N[0]) );
    int M = (int)( sizeof(cjto_M) / sizeof(cjto_M[0]) );

    double * x = (double*) malloc( 1 * sizeof(double) );
    double * x_aux;

    double Ysample;
    double Ymean;

    double auxMean;
    double auxRange;
    double sumMean;
    double SVR;

    double MM;
    double MDR;

    double c4;
    double SD;

    double sumVar;

    int c;
    double v;

    double MVR;
    int SomaPontosForaTotal;
    int SomaPontosFora;
    double alpha;
    double ARL;

    double LSC;
    double LIC;

    float * meanArray = (float*) malloc( jj * sizeof(float) );
    float * varArray = (float*) malloc( jj * sizeof(float) );

    float * somaTodosArray = (float*) malloc( jj * sizeof(float) );
    float * pc50Array = (float*) malloc( jj * sizeof(float) );
    float * pc100Array = (float*) malloc( jj * sizeof(float) );
    float * pc200Array = (float*) malloc( jj * sizeof(float) );


    //CREATING THE FINAL FILE FOR THE RESULTS
    ofstream outputFile;

    switch( sde )
    {
        case 1:
            outputFile.open ("finalResultsSD1.xls");
            break;
        case 2:
            outputFile.open ("finalResultsSD2.xls");
            break;
        case 3:
            outputFile.open ("finalResultsSD3.xls");
            break;
        case 4:
            outputFile.open ("finalResultsSD4.xls");
            break;
        case 5:
            outputFile.open ("finalResultsSD5.xls");
            break;
        default:
            exit(0);
    }

    outputFile << endl << "J\t" << "K\t" << "M\t" << "N\t" << "mean(ACRL)\t" << "var\t" << "std dev\t" ;
    outputFile << "Total/User\t" << "<50\t" << "<100\t" << "<200\t";
    outputFile << "Total/User G\t" << "<50 G\t" << "<100 G\t" << "<200 G\n";

    //CREATING THE BASE TO THE RESULT FILES
    string nomeBaseArquivo ("individualFinalResults");
    stringstream nomeArquivo;
    ofstream arquivoEscritaCenarios;


    int flag = 0; // identify when a point is inside the control lines
    int * frequency;
    int * frequencyArrayGeneral;
    int * more_numbers;
    int maxVectorSize = 0;
    int soma50 = 0;
    int soma100 = 0;
    int soma200 = 0;
    int somaTodos = 0;

    float mediaRLusuario, dpRLusuario, SUMmediaRLusuario, varRLusuario, mediaMediaRLusuario = 0;

    float somaGeral = 0;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);    // create the random number generator
    std::normal_distribution<double> distribution( 0.0, 1.0 );

    for( int _n = 0; _n < N; _n++ )
    {
        // refresh the nn index using a value from the set "cjto_N" previously defined
        nn = cjto_N[_n];

        x_aux = (double*) realloc ( x, nn * sizeof(double));
        if ( x_aux != NULL )
        {
            x = x_aux;
        }

        for( int _m = 0; _m < M; _m++ )
        {
            // atualiza o mm com o valor vindo do conjunto
            mm = cjto_M[_m];

            // reseting the vector length that accumulates the frequency RL
            maxVectorSize = 0;
            flag = 0;
            frequency = (int*) malloc( 1 * sizeof(int) );
            frequencyArrayGeneral = (int*) malloc( 1 * sizeof(int) );

            nomeArquivo << nomeBaseArquivo << "_SD_" << sde << "_j_" << jj << "_k_" << kk << "_m_" << cjto_M[_m] << "_n_" << cjto_N[_n] << ".xls" ;
            arquivoEscritaCenarios.open (nomeArquivo.str());
            arquivoEscritaCenarios << "J\t" << "K\t" << "M\t" << "N\t" << "mean(ACRL)\t" << "var\t" << "std dev\t" << "Total/User\t";
            arquivoEscritaCenarios << "<50\t" << "<100\t" << "<200\t" << "alpha\t" << "ARL\t" << "\n";

            // to count the total number of samples out of the control lines
            SomaPontosForaTotal = 0;
            for( int s = 0; s < jj; s++ )
            {
                //// When the practitioner is changed, it is needed to reset the point counting
                flag = 0;
                sumMean = 0;
                sumVar = 0;
                switch( sde )
                {
                    case 1:
                        // Beginning of the code for the SD1

                        //// Calculating the mean
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumMean += auxMean;
                        }
                        MM = sumMean / mm;
                        auxRange = 0;
                        //// Calculating standard deviation
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxRange += RangeD( x, nn );
                        }
                        SVR = auxRange / mm;
                        if( nn == 5 )
                        {
                            SD = SVR / 2.326;  //////// d2
                        }
                        else if( nn == 10 )
                        {
                            SD = SVR / 3.078;  //////// d2
                        }
                        // End of the code for the SD1
                        break;

                    case 2:
                        // Beginning of the code for the SD2

                        //// Calculating the mean
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumMean += auxMean;
                        }
                        MM = sumMean / mm;
                        SVR = 0;
                        //// Calculating standard deviation
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );

                            SVR += DeviationD( x, nn, auxMean ); //// Calculating standard deviation
                        }
                        MDR = SVR / mm;
                        c4 = ((double)( 4 * nn - 4 )) / ((double)( 4 * nn - 3 ));
                        SD = MDR / c4;
                        // End of the code for the SD2
                        break;

                    case 3:
                        // Beginning of the code for the SD3

                        //// Calculating mean
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumMean += auxMean;
                        }
                        MM = sumMean / mm;
                        //// Calculating standard deviation
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumVar += VarianceD( x, nn, auxMean );
                        }
                        SVR = sumVar / mm;
                        v = mm * ( nn - 1 );
                        c4 = ( 4 * v ) / ( 4 * v + 1 );
                        SD = ( sqrt(SVR) )/ c4;  //////// SD3

                        // End of the code for the SD3
                        break;

                    case 4:
                        // Beginning of the code for the SD4


                        //// Calculating mean
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumMean += auxMean;
                        }
                        MM = sumMean / mm;
                        //// Calculating standard deviation
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumVar += VarianceD( x, nn, auxMean );
                        }
                        SVR = sumVar / mm;
                        v = mm * ( nn - 1 );
                        c4 = ( 4 * v ) / ( 4 * v + 1 );
                        SD = c4 * ( sqrt(SVR) );  //////// SD4

                        // End of the code for the SD4
                        break;

                    case 5:
                        // Beginning of the code for the SD5


                        sqrt( MVR );
                        //// Calculating the mean
                        for( int t = 0; t < mm; t++ )
                        {
                            for( int g = 0; g < nn; g++ )
                            {
                                x[g] = distribution(generator);
                            }
                            auxMean = AverageD( x, nn );
                            sumMean += auxMean;
                            sumVar += VarianceD( x, nn, auxMean );
                        }
                        MM = sumMean / mm;
                        MVR = sumVar / mm;
                        SD = sqrt( MVR );

                        // End of the code for the SD5
                        break;
                    default:
                        exit(0);
                }

                LSC = MM + 3.0 * SD / sqrt( nn );
                LIC = MM - 3.0 * SD / sqrt( nn );

                // FASE 2

                for( int i = 0; i < kk; i++ )
                {

                    Ysample = 0;

                    for( int g = 0; g < nn; g++ )
                    {
                        Ysample += distribution(generator);
                    }
                    Ymean = Ysample / nn;

                    if ( LIC > Ymean || Ymean > LSC )
                    {

                        if( flag > maxVectorSize )
                        {
                            //// if the frequency between two points outside the control is a new high number, so the array must be increased
                            more_numbers = (int*) realloc ( frequency, (flag + 1) * sizeof(int));

                            //// if the array increased, all intermediate possible frequencies are set to zero
                            if ( more_numbers != NULL ) {
                                frequency = more_numbers;

                                //// Resize the array to define the frequency
                                more_numbers = (int*) realloc ( frequencyArrayGeneral, (flag + 1) * sizeof(int));
                                frequencyArrayGeneral = more_numbers;

                                //// Reseting the intermediate elements of frequency
                                for( int a = maxVectorSize; a < flag; a++ )
                                {
                                    frequency[a] = 0;
                                    frequencyArrayGeneral[a] = 0;
                                }
                            }
                            //// Save the number of higher observed frequency
                            maxVectorSize = flag;
                        }
                        /// incrementa a freuqencia observada
                        frequency[flag]++;

                        flag = 0;
                    }
                    else
                    {
                        //// The point is inside the control
                        flag++;
                    }
                }

                //// Calculating mean and standard deviation
                mediaRLusuario = 0;
                SUMmediaRLusuario = 0.0;
                for( int a = 0; a < maxVectorSize; a++ )
                {
                    if ( frequency[a] >= 0 )
                    {
                        mediaRLusuario += frequency[a] * ( a + 1 );
                        SUMmediaRLusuario += frequency[a];
                    }
                }

                mediaRLusuario = mediaRLusuario / SUMmediaRLusuario;

                // Save the mean for each user in the array
                meanArray[s] = mediaRLusuario;

                // Calculating the total mean
                if ( !isnan( mediaRLusuario ))
                    mediaMediaRLusuario += mediaRLusuario;

                //// Counting the total number of points that are out of control
                if ( !isnan( SUMmediaRLusuario ))
                    SomaPontosForaTotal += SUMmediaRLusuario;

                dpRLusuario = 0.0;
                for( int a = 0; a < maxVectorSize; a++ )
                {
                    if ( frequency[a] >= 0 )
                        dpRLusuario += frequency[a] * pow( ((double)( a + 1 )) - mediaRLusuario, 2);
                }

                varRLusuario = dpRLusuario / SUMmediaRLusuario;

                // Storing the variance in the array
                varArray[s] = varRLusuario;

                if ( !isnan( varRLusuario ))
                    somaGeral += varRLusuario;

                //// Printing RL in file //////
                soma50 = 0;
                soma100 = 0;
                soma200 = 0;
                somaTodos = 0;

                for( int a = 0; a < maxVectorSize; a++ )
                {
                    if ( frequency[a] >= 0 )
                    {
                        somaTodos += frequency[a];
                        if( a < 50 )
                            soma50 += frequency[a];
                        if( a < 100 )
                            soma100 += frequency[a];
                        if( a < 200 )
                            soma200 += frequency[a];

                        frequencyArrayGeneral[a] += frequency[a];

                    }
                    frequency[a] = 0;
                }

                somaTodosArray[s] = somaTodos;
                pc50Array[s] = ((float)soma50)/somaTodos;
                pc100Array[s] = ((float)soma100)/somaTodos;
                pc200Array[s] = ((float)soma200)/somaTodos;

                arquivoEscritaCenarios << s + 1 << "\t" << kk << "\t" << mm << "\t" << nn << "\t" ;
                arquivoEscritaCenarios << noshowpoint << mediaRLusuario << "\t" << varRLusuario << "\t" << sqrt(varRLusuario) << "\t" << somaTodos << "\t";
                arquivoEscritaCenarios << pc50Array[s] << "\t" << pc100Array[s] << "\t" << pc200Array[s] << "\t";
                if( somaTodos > 0 )
                    arquivoEscritaCenarios << ((float)somaTodos)/kk << "\t" << kk/somaTodos << endl;
                else
                    arquivoEscritaCenarios << ((float)somaTodos)/kk << "\t0.0" << endl;
            }

            free( frequency );
            //// Printing RL in file //////
            soma50 = 0;
            soma100 = 0;
            soma200 = 0;
            somaTodos = 0;
            for( int a = 0; a < maxVectorSize; a++ )
            {
                somaTodos += frequencyArrayGeneral[a];
                if( a < 50 )
                    soma50 += frequencyArrayGeneral[a];
                if( a < 100 )
                    soma100 += frequencyArrayGeneral[a];
                if( a < 200 )
                    soma200 += frequencyArrayGeneral[a];
            }

            //// Calculating mean and standard deviation
            SUMmediaRLusuario = 0.0;
            mediaRLusuario = 0.0;
            for( int a = 0; a < maxVectorSize; a++ )
            {
                if( frequencyArrayGeneral[a] >= 0 )
                {
                    mediaRLusuario += frequencyArrayGeneral[a] * ( a + 1 );
                    SUMmediaRLusuario += frequencyArrayGeneral[a];
                }
            }
            mediaRLusuario = mediaRLusuario / ( SUMmediaRLusuario );

            dpRLusuario = 0.0;
            for( int a = 0; a < maxVectorSize; a++ )
            {
                if( frequencyArrayGeneral[a] >= 0 )
                    dpRLusuario += frequencyArrayGeneral[a] * pow( ((double)( a + 1 )) - mediaRLusuario, 2);
            }
            varRLusuario = dpRLusuario / SUMmediaRLusuario;

            dpRLusuario = sqrt( varRLusuario );


            float auxf1 = 0, auxf2 = 0;
            int nanMean = 0, nanVar = 0, nan50 = 0, nan100 = 0, nan200 = 0;

            float auxSumT = 0.0, auxPc50 = 0.0, auxPc100 = 0.0, auxPc200 = 0.0;
            for( int s = 0; s < jj; s++ )
            {
                if( isnan( meanArray[s] ) )
                    nanMean++;
                else
                    auxf1 += meanArray[s];

                if( isnan( varArray[s] ) )
                    nanVar++;
                else
                    auxf2 += varArray[s];

                if( isnan( somaTodosArray[s] ) )
                    nanMean++;
                else
                    auxSumT += somaTodosArray[s];

                if( isnan( pc50Array[s] ) )
                    nan50++;
                else
                    auxPc50 += pc50Array[s];

                if( isnan( pc100Array[s] ) )
                    nan100++;
                else
                    auxPc100 += pc100Array[s];

                if( isnan( pc200Array[s] ) )
                    nan200++;
                else
                    auxPc200 += pc200Array[s];

            }

            sorting( pc50Array, jj );
            sorting( pc100Array, jj );
            sorting( pc200Array, jj );

            auxf1 = auxf1 / (jj);
            auxf2 = auxf2 / (jj);

            auxSumT = auxSumT/ (jj);
            auxPc50 = auxPc50/ (jj);
            auxPc100 = auxPc100/ (jj);
            auxPc200 = auxPc200/ (jj);


            arquivoEscritaCenarios << endl << "J\t" << "K\t" << "M\t" << "N\t" << "mean(ACRL)\t" << "var\t" << "std dev\t" << "Total/User\t";
            arquivoEscritaCenarios << "<50\t" << "<100\t" << "<200\t" << "alpha\t" << "ARL\t" << "\n";


            arquivoEscritaCenarios << endl << "J\t" << "K\t" << "M\t" << "N\t" << "mean(ACRL)\t" << "var\t" << "std dev\t" ;
            arquivoEscritaCenarios << "Total/User\t" << "<50\t" << "<100\t" << "<200"  << "\t";
            arquivoEscritaCenarios << "Total/User G\t" << "<50 G\t" << "<100 G\t" << "<200 G\n";

            arquivoEscritaCenarios << jj << "\t" << kk << "\t" << mm << "\t" << nn << "\t" ;
            arquivoEscritaCenarios << auxf1 << "\t" << auxf2 << "\t" << sqrt( auxf2 ) << "\t";
            arquivoEscritaCenarios << somaTodos << "\t" << ((float)soma50)/somaTodos << "\t" << ((float)soma100)/somaTodos << "\t" << ((float)soma200)/somaTodos << "\t" ;
            arquivoEscritaCenarios << auxSumT << "\t" << auxPc50 << "\t" << auxPc100 << "\t" << auxPc200 << "\t" ;

            free( frequencyArrayGeneral );

            arquivoEscritaCenarios.close();
            nomeArquivo.clear();
            nomeArquivo.str(string());

            outputFile << jj << "\t" << kk << "\t" << mm << "\t" << nn << "\t" ;
            outputFile << auxf1 << "\t" << auxf2 << "\t" << sqrt( auxf2 ) << "\t";
            outputFile << somaTodos << "\t" << ((float)soma50)/somaTodos << "\t" << ((float)soma100)/somaTodos << "\t" << ((float)soma200)/somaTodos << "\t" ;
            outputFile << auxSumT << "\t" << auxPc50 << "\t" << auxPc100 << "\t" << auxPc200 << "\n" ;


            for( int s = 0; s < jj; s++ )
            {

                meanArray[s] = 0.0;
                varArray[s] = 0.0;

                somaTodosArray[s] = 0.0;
                pc50Array[s] = 0.0;
                pc100Array[s] = 0.0;
                pc200Array[s] = 0.0;
            }
        }
    }

    outputFile.close();

    free( x );
    free( meanArray );
    free( varArray );

    free( somaTodosArray );
    free( pc50Array );
    free( pc100Array );
    free( pc200Array );

    system("pause");
    return 0;
}
