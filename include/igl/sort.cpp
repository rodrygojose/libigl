#include "sort.h"

#include "SortableRow.h"
#include "reorder.h"
#include "IndexComparison.h"

#include <cassert>
#include <algorithm>

template <typename DerivedX, typename DerivedIX>
IGL_INLINE void igl::sort(
  const Eigen::PlainObjectBase<DerivedX>& X,
  const int dim,
  const bool ascending,
  Eigen::PlainObjectBase<DerivedX>& Y,
  Eigen::PlainObjectBase<DerivedIX>& IX)
{
  using namespace Eigen;
  // dim must be 2 or 1
  assert(dim == 1 || dim == 2);
  // Resize output
  Y.resize(X.rows(),X.cols());
  IX.resize(X.rows(),X.cols());
  // idea is to process each column (or row) as a std vector
  // get number of columns (or rows)
  int num_outer = (dim == 1 ? X.cols() : X.rows() );
  // get number of rows (or columns)
  int num_inner = (dim == 1 ? X.rows() : X.cols() );
  // loop over columns (or rows)
  for(int i = 0; i<num_outer;i++)
  {
    // Unsorted index map for this column (or row)
    std::vector<size_t> index_map(num_inner);
    std::vector<double> data(num_inner);
    for(int j = 0;j<num_inner;j++)
    {
      if(dim == 1)
      {
        data[j] = (double) X(j,i);
      }else
      {
        data[j] = (double) X(i,j);
      }
    }
    // sort this column (or row)
    igl::sort( data, ascending, data, index_map);
    // Copy into Y and IX
    for(int j = 0;j<num_inner;j++)
    {
      if(dim == 1)
      {
        Y(j,i) = data[j];
        IX(j,i) = index_map[j];
      }else
      {
        Y(i,j) = data[j];
        IX(i,j) = index_map[j];
      }
    }
  }
}

template <class T>
IGL_INLINE void igl::sort(
  const std::vector<T> & unsorted,
  const bool ascending,
  std::vector<T> & sorted,
  std::vector<size_t> & index_map)
{
  // Original unsorted index map
  index_map.resize(unsorted.size());
  for(size_t i=0;i<unsorted.size();i++)
  {
    index_map[i] = i;
  }
  // Sort the index map, using unsorted for comparison
  std::sort(
    index_map.begin(),
    index_map.end(),
    igl::IndexLessThan<const std::vector<T>& >(unsorted));

  // if not ascending then reverse
  if(!ascending)
  {
    std::reverse(index_map.begin(),index_map.end());
  }
  // make space for output without clobbering
  sorted.resize(unsorted.size());
  // reorder unsorted into sorted using index map
  igl::reorder(unsorted,index_map,sorted);
}

#ifndef IGL_HEADER_ONLY
// Explicit template specialization
// generated by autoexplicit.sh
template void igl::sort<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, int, bool, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::sort<Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, int, bool, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::sort<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > >(std::vector<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > > > const&, bool, std::vector<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<int, -1, 1, 0, -1, 1> > > >&, std::vector<unsigned long, std::allocator<unsigned long> >&);
template void igl::sort<int>(std::vector<int, std::allocator<int> > const&, bool, std::vector<int, std::allocator<int> >&, std::vector<unsigned long, std::allocator<unsigned long> >&);
template void igl::sort<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > >(std::vector<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > > > const&, bool, std::vector<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> >, std::allocator<SortableRow<Eigen::Matrix<double, -1, 1, 0, -1, 1> > > >&, std::vector<unsigned long, std::allocator<unsigned long> >&);
#endif
