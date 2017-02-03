// Copyright 2016 Roberto Francescon
// This file is part of freeRaptor.
//
// freeRaptor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// freeRaptor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with freeRaptor.  If not, see <http://www.gnu.org/licenses/>.

#include <gf2matrix.h>

GF2mat::GF2mat(int n_rows, int n_cols)
  :
  n_row(n_rows), 
  n_col(n_cols)
{
  //n_word = (n_row + wordsize-1) >> wordmasksize;
  n_word = (n_col + wordsize-1) >> wordmasksize;

  //cols = (uint32_t**) calloc(n_col, sizeof *cols);
  rows = (uint32_t**) calloc(n_col, sizeof *rows);
  //gf2mat = (uint32_t*) calloc(n_word*n_col, sizeof *gf2mat);
  gf2mat = (uint32_t*) calloc(n_word*n_row, sizeof *gf2mat);

  //Assign columns pointers: leap n_word words at a time
  for (int i = 0; i < n_row; i++)
    {
      //cols[i] = gf2mat  + i*(n_word);
      rows[i] = gf2mat  + i*(n_word);
    }
}

GF2mat::~GF2mat(){}

int GF2mat::get_nrows()
{
  return n_row;
}

int GF2mat::get_ncols()
{
  return n_col;
}

int GF2mat::get_entry(int row, int col)
{
  //return ((cols[col][row>>wordmasksize]) >> (row&wordmask)) & 1;
  return ((rows[row][col>>wordmasksize]) >> (col&wordmask)) & 1;
}

void GF2mat::set_entry(int row, int col, int val)
{
  //(cols[col][row>>wordmasksize]) ^= (-val ^ (cols[col][row>>wordmasksize])) & (1 << (row&wordmask));
  (rows[row][col>>wordmasksize]) ^= (-val ^ (rows[row][col>>wordmasksize])) & (1 << (col&wordmask));
}

void GF2mat::row_exchange(int a, int b)
{
  uint32_t* temp = rows[a];
  rows[a] = rows[b];
  rows[b] = temp;
}

void GF2mat::invert_GE()
{
  int i;
  int j;
  for (i = 0; i < n_row; i++)
    {
      j = i;
      while(j < (n_row-1) && get_entry(j, i) == 0)
  	  j++;
      if (i != j)
  	row_exchange(i, j);
      for (int k=0; k < n_row; k++)
	{
	  if (k != i && get_entry(k, i) == 1)
	    {
	      *rows[k] = *rows[k] ^ *rows[i];
	      // for (int l=0; l < n_col; l++)
	      // 	set_entry(k, l, (get_entry(k,l)) ^ (get_entry(i,l)));
	    }
	}
    }
}

void GF2mat::print()
{
  for (int i = 0; i < n_row; i++)
    {
      for (int j = 0; j < n_col; j++)
	{
	  std::cout << get_entry(i,j) << " ";
	}
      std::cout << "\n";
    }
  return;
}
