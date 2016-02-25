#include "../inc/math.hpp" // <math.hpp>
#include "../inc/tree.hpp" // <tree.hpp>

int main() 
{
  tree::CEntry* pEntry0  = new tree::CEntry({  1.0f, 2.0f,  3.0f, 10.0f, 7.0f, 9.5f });
  tree::CEntry* pEntry1  = new tree::CEntry({  0.0f, 1.0f,  4.0f,  2.0f, 3.0f, 2.5f });
  tree::CEntry* pEntry2  = new tree::CEntry({ 15.0f, 9.0f, 12.0f,  3.0f, 1.0f, 1.5f });
  
  tree::COcTree* pOcTree = new tree::COcTree;
  
  try 
  {
    pOcTree->insert(pEntry0);
    pOcTree->insert(pEntry1);
    pOcTree->insert(pEntry2);
  } 
  catch (tree::COutOfBoundsException& e)
  {
    std::cout << e.what() << std::endl;
  }
  
  delete pOcTree;
  delete pEntry0;
  delete pEntry1;
  delete pEntry2;
  
  return 0;
}