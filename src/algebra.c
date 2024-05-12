#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    int i,j;
    if(a.rows != b.rows || a.cols != b.cols){
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix c= create_matrix(a.rows,a.cols);
    for(i=0;i<a.rows;i++)
    {
        for(j=0;j<a.cols;j++)
        {
            c.data[i][j]=a.data[i][j]+b.data[i][j];
       }
        }
    return c;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    int i,j;
    if(a.rows != b.rows || a.cols != b.cols){
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
     Matrix c= create_matrix(a.rows,a.cols);
    for(i=0;i<a.rows;i++)
    {
        for(j=0;j<a.cols;j++)
        {
               c.data[i][j]=a.data[i][j]-b.data[i][j];
        }
    }
    return c;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    int i,j,k;
    if (a.cols!=b.rows){
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
    Matrix c = create_matrix(a.rows,b.cols);
    for(i=0;i<a.rows;i++)
    {
        for(j=0;j<b.cols;j++)
        {
            for(k=0;k<a.cols;k++)
            {
                c.data[i][j] += a.data[i][k]*b.data[k][j];
            }
        }
    }
    return c;

}

Matrix scale_matrix(Matrix a, double k)
{   
    int i,j;
    Matrix b= create_matrix(a.rows,a.cols);
    for(i=0;i<a.rows;i++)
    {
        for(j=0;j<a.cols;j++)
        {
            b.data[i][j]=a.data[i][j]*k;
        }
    }
    
    return b;
}

Matrix transpose_matrix(Matrix a)
{
    int i,j;
    Matrix c= create_matrix(a.cols,a.rows);
   for (i = 0; i < a.rows; i++)//循环写入newMat
	{
		for (j = 0; j < a.cols; j++)
		{
			c.data[j][i] = a.data[i][j];//置换
		}
	}
    return c;
}

double det_matrix(Matrix a)
{
    int i,j,k;
    double sum=0;
    if (a.rows==a.cols&&a.rows>2)
    {
        Matrix b= create_matrix(a.rows-1,a.cols-1); /*取第一行*/
        for(i=0;i<a.rows;i++)
        {
            for(j=0;j<a.rows-1;j++)
            {
                for(k=0;k<a.rows-1;k++){
                    if (j<i)
                    {
                        b.data[i][j]=a.data[i+1][j];
                    }
                   else
                   {
                        b.data[i][j]=a.data[i+1][j+1];
                   }
                }
            }
            int m,mult=1;
            for(m=0;m<i;m++)
            {
                mult=mult*(-1);
            }
            sum=a.data[0][i]*mult*det_matrix(b);
                
        }
        
        
    }
    else if (a.rows=2)
    {
        sum=a.data[0][0]*a.data[1][1]-a.data[1][0]*a.data[0][1];
    }
    else if (a.rows=1)
    {
        sum=a.data[0][0];
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
    }
    return sum;
}

Matrix inv_matrix(Matrix a)
{
    int i, j, k;
	double max, temp;
	Matrix b = create_matrix(a.cols,a.rows);                //临时矩阵
	Matrix c = create_matrix(a.cols,a.rows);							  //将A矩阵存放在临时矩阵B中
    if(a.cols != a.rows){
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
	for (i = 0; i < a.cols; i++)
	{
		for (j = 0; j < a.cols; j++)
		{
			b.data[i][j] = a.data[i][j];
		}
	}
	//初始化C矩阵为单位阵
	for (i = 0; i < a.cols; i++)
	{
		for (j = 0; j < a.cols; j++)
		{
			c.data[i][j] = (i == j) ? (double)1 : 0;
		}
	}
	for (i = 0; i < a.cols; i++)
	{
		//寻找主元
		max = b.data[i][i];
		k = i;
		for (j = i + 1; j < a.cols; j++)
		{
			if (fabs(b.data[j][i]) > fabs(max))
			{
				max = b.data[j][i];
				k = j;
			}
		}
		//如果主元所在行不是第i行，进行行交换
		if (k != i)
		{
			for (j = 0; j < a.cols; j++)
			{
				temp = b.data[i][j];
				b.data[i][j] = b.data[k][j];
				b.data[k][j] = temp;
				//B伴随交换
				temp = c.data[i][j];
				c.data[i][j] = c.data[k][j];
				c.data[k][j] = temp;
			}
		}
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
		if (b.data[i][i] == 0)
		{
			printf("Error: The matrix is singular.\n");
			return create_matrix(0, 0);;
		}
		//消去A的第i列除去i行以外的各行元素
		temp = b.data[i][i];
		for (j = 0; j < a.cols; j++)
		{
			b.data[i][j] = b.data[i][j] / temp;        //主对角线上的元素变为1
			c.data[i][j] = c.data[i][j] / temp;        //伴随计算
		}
		for (j = 0; j < a.cols; j++)        //第0行->第n行
		{
			if (j != i)                //不是第i行
			{
				temp = b.data[j][i];
				for (k = 0; k < a.cols; k++)        //第j行元素 - i行元素*j列i行元素
				{
					b.data[j][k] = b.data[j][k] - b.data[i][k] * temp;
					c.data[j][k] = c.data[j][k] - c.data[i][k] * temp;
				}
			}
		}
	}
    return c;
}

int rank_matrix(Matrix a)
{
    int i,j,k;
	float temp;  
	int r=0,d=0; //r表示秩，d表示当前正在哪一行。
	for(i=0;i<a.cols;i++)
	{
		k=d;                            /*a[i][i] a[i+1][i] ... a[n][i]中绝对值最大的行位置*/
		for(j=d+1;j<a.rows;j++)
			if(fabs(a.data[k][i])<fabs(a.data[j][i]))
				k=j;	
		if(k!=d)                     /*交换第i行和第k行，行列式该变号*/
		{
			for(j=i;j<a.cols;j++)
			{
				temp=a.data[d][j];
			    a.data[d][j]=a.data[k][j];
		 	    a.data[k][j]=temp;
			}
		}
		if(a.data[d][i]==0)            /*当a[i][i]为零是时，行列式为零*/     
		{ 
			continue;
		}
		else
		{
			r=r+1;
			for(j=0;j<a.rows;j++)
			{
				if(j!=d)
				{
					temp=-1*a.data[j][i]/a.data[d][i];
					for(k=i;k<a.cols;k++)
						a.data[j][k]=a.data[j][k]+temp*a.data[d][k];
				 }
			 }
			temp=a.data[d][i];
			for(j=i;j<a.cols;j++)
				a.data[d][j]=a.data[d][j]/temp;
		}
		d=d+1;
		if(d>=a.rows)
			break;
    }
    return r;
}

double trace_matrix(Matrix a)
{
    int i,j;
    double trace=0;
    if(a.cols != a.rows){
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    for(i=0;i<a.rows;i++)
        {
            for(j=0;j<a.rows;j++)
            {
                if (i==j)
                {
                       trace += a.data[i][j];
                }
            }
        }
    return trace;
}

void print_matrix(Matrix a)
{
    int i,j;
    for (i = 0; i < a.rows; i++)
    {
        for (j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}