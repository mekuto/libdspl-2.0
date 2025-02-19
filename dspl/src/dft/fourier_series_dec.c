/*
* Copyright (c) 2015-2019 Sergey Bakhurin
* Digital Signal Processing Library [http://dsplib.org]
*
* This file is part of libdspl-2.0.
*
* is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser    General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSPL is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with Foobar.    If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dspl.h"



#ifdef DOXYGEN_ENGLISH
/*! ****************************************************************************
\ingroup DFT_GROUP
\fn int fourier_series_dec(double* t, double* s, int nt, double period,
                           int nw, double* w, complex_t* y)

\brief
Fourier series coefficient calculation for periodic signal

\param[in] t
Pointer to the time vector. \n
Vector size is `[nt x 1]`. \n
\n

\param[in] s
Pointer to the signal corresponds to time `t`. \n
Vector size is `[nt x 1]`. \n
\n

\param[in] nt
Size of time and signal vectors. \n
This value must be positive. \n 
\n

\param[in] period
Signal time period. \n 
\n

\param[in] nw
Number of Fourie series coefficients. \n 
\n

\param[out] w
Pointer to the frequency vector (rad/s). \n
Vector size is `[nw x 1]`. \n
Memory must be allocated. \n
\n

\param[out] y
Pointer to the complex Fourier series coefficients vector. \n
Vector size is `[nw x 1]`. \n
Memory must be allocated. \n
\n

\return
`RES_OK` if function is calculated successfully. \n 
Else \ref ERROR_CODE_GROUP "code error".

\note
Numerical integration is used for Fourier series coefficients calculation.
This function is not effective. 
To increase the speed of calculation of the signal spectrum
it is more expedient to use fast Fourier transform algorithms.
\n

\author Sergey Bakhurin www.dsplib.org
***************************************************************************** */
#endif
#ifdef DOXYGEN_RUSSIAN
/*! ****************************************************************************
\ingroup DFT_GROUP
\fn int fourier_series_dec(double* t, double* s, int nt, double period,
                           int nw, double* w, complex_t* y)

\brief
Расчет коэффициентов разложения в ряд Фурье

Функция рассчитывает спектр периодического сигнала при усечении ряда Фурье \n


\param[in] t
Указатель на массив моментов времени дискретизации исходного сигнала `s`. \n
Размер вектора вектора `[nt x 1]`. \n
Память должна быть выделена. \n 
\n

\param[in] s
Указатель на массив значений исходного сигнала`s`. \n
Размер вектора `[nt x 1]`. \n
Память должна быть выделена. \n 
\n


\param[in] nt
Размер выборки исходного сигнала. \n
Значение должно быть положительным. \n 
\n

\param[in] period
Период повторения сигнала. \n 
\n

\param[in] nw
Размер усеченного ряда Фурье. \n 
\n

\param[out] w
Указатель на массив частот спектра периодического сигнала. \n
Размер вектора `[nw x 1]`. \n
Память должна быть выделена. \n
\n

\param[out] y
Указатель массив комплексных значений спектра периодического сигнала. \n
Размер вектора `[nw x 1]`. \n
Память должна быть выделена. \n 
\n

\return
`RES_OK` --- коэффициенты ряда Фурье рассчитаны успешно. \n
В противном случае \ref ERROR_CODE_GROUP "код ошибки". \n

\note
Для расчета спектра сигнала используется численное интегрирование
исходного сигнала методом трапеций. Данная функция не является
эффективной. Для увеличения скорости расчета спектра сигнала
целесообразнее использовать алгоритмы дискретного
и быстрого преобразования Фурье.
\n

\author Бахурин Сергей www.dsplib.org
***************************************************************************** */
#endif
int DSPL_API fourier_series_dec(double* t, double* s, int nt, double period,
                                                                int nw, double* w, complex_t* y)
{
    int k, m;
    double dw = M_2PI / period;
    complex_t e[2];

    if(!t || !s || !w || !y)
        return ERROR_PTR;
    if(nt<1 || nw < 1)
        return ERROR_SIZE;
    if(period <= 0.0)
        return ERROR_NEGATIVE;

    memset(y, 0 , nw*sizeof(complex_t));

    for(k = 0; k < nw; k++)
    {
        w[k] = (k - nw/2) * dw;
        RE(e[1]) =    s[0] * cos(w[k] * t[0]);
        IM(e[1]) = -s[0] * sin(w[k] * t[0]);
        for(m = 1; m < nt; m++)
        {
            RE(e[0]) = RE(e[1]);
            IM(e[0]) = IM(e[1]);
            RE(e[1]) =     s[m] * cos(w[k] * t[m]);
            IM(e[1]) = - s[m] * sin(w[k] * t[m]);
            RE(y[k]) += 0.5 * (RE(e[0]) + RE(e[1]))*(t[m] - t[m-1]);
            IM(y[k]) += 0.5 * (IM(e[0]) + IM(e[1]))*(t[m] - t[m-1]);
        }
        RE(y[k]) /= period;
        IM(y[k]) /= period;
    }

    if(!(nw%2))
        RE(y[0]) = RE(y[1]) = 0.0;

    return RES_OK;
}
