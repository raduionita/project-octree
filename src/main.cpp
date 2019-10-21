#include "../inc/math.hpp" // <math.hpp>
#include "../inc/tree.hpp" // <tree.hpp>
#include <iostream>
#include <ostream>

int main()
{
  tree::CEntry* pEntry0  = new tree::CEntry({  1.0f, 2.0f,  3.0f, 10.0f, 7.0f, 9.5f });
  tree::CEntry* pEntry1  = new tree::CEntry({  0.0f, 1.0f,  4.0f,  2.0f, 3.0f, 2.5f });
  tree::CEntry* pEntry2  = new tree::CEntry({ 15.0f, 9.0f, 12.0f,  3.0f, 1.0f, 1.5f });

  // sdasd

  tree::COcTree* pOcTree = new tree::COcTree;

  std::cout << "test from gitpod.io" << std::endl;

  try
  {
    pOcTree->insert(pEntry0);
    pOcTree->insert(pEntry2);
    pOcTree->insert(pEntry1);
  }
  catch (tree::COutOfBoundsException& e)
  {
    std::cout << e.what() << std::endl;
  }

  pOcTree->update();
  const auto& pRoot = pOcTree->getRoot();

  delete pOcTree;
  delete pEntry0;
  delete pEntry1;
  delete pEntry2;

  return 0;
}
