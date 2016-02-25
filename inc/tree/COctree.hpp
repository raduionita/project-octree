#ifndef __tree_coctree_hpp__
#define __tree_coctree_hpp__

#include <iostream>
#include <initializer_list>
#include <cassert>
#include <array>
#include <vector>
#include <exception>
#include <string>

#include "../math.hpp"   // <math.hpp>
#include "../util.hpp" // <util.hpp>

namespace tree
{
  class CException : public std::exception
  {
    protected:
    std::string mMessage;
    
    public:
    CException(const std::string& message) : mMessage(message)
    {
      
    }
    
    public:
    virtual const char* what()
    {
      return mMessage.c_str();
    }
  };

  class COutOfBoundsException : public CException
  {
    public:
    COutOfBoundsException(const std::string& message) : CException(message)
    {
      
    }
  };

  class CEntry
  {
    public:
    math::CBox mAABB;
    
    public:
    CEntry()
    {
    
    }
    
    CEntry(std::initializer_list<math::real> list)
    {
      if(list.size() == 6)
      {
        std::initializer_list<math::real>::iterator it;
        size_t i = 0;
        for(it = list.begin(); it != list.end(); ++it)
        {
          if(i == 0) mAABB.mPosition.x = *it;
          if(i == 1) mAABB.mPosition.y = *it;
          if(i == 2) mAABB.mPosition.z = *it;
          if(i == 3) mAABB.mWidth  = *it;
          if(i == 4) mAABB.mHeight = *it;
          if(i == 5) mAABB.mDepth  = *it;
          ++i;
        }
      }
    }
    
    public:
    friend std::ostream& operator << (std::ostream& out, const CEntry& oEntry);
  };
  
  std::ostream& operator << (std::ostream& out, const CEntry& oEntry)
  {
    out << "CEntry[" << oEntry.mAABB << "]";
    return out;
  }
  
  class CNode
  {
    protected:
    static unsigned int  sId;
    
    public:
    unsigned int          mId;
    CNode*                mParent;
    std::array<CNode*, 8> mChildren; // 8 children
    std::vector<CEntry*>  mEntries;  // data
    math::CBox            mAABB;
    bool                  mDirty;
    
    public:
    CNode(CNode* pParent, const math::CBox& oAABB = math::CBox(0.0f))
    {
      mId     = ++CNode::sId;
      mParent = pParent;
      mChildren.fill(nullptr);
      mAABB   = oAABB;
      mDirty  = true;
      
      std::cout << "\t" << "CNode["<< mId <<"]::CNode(CNode*, math::CBox)" << std::endl;
      
      // @todo:
      // CNodePool
    }
    
    virtual ~CNode()
    {
      std::cout << "\t" << "CNode["<< mId <<"]::~CNode()" << std::endl;
      
      mEntries.empty();
      for(auto pChild : mChildren)
      {
        delete pChild;
        pChild = nullptr; 
        // @todo
        // put back into CNodePool
      }
    }

    protected:
    CNode*& getNode(size_t i)
    {
      assert(i < 8);
      CNode*& pChild = mChildren[i];
      if(pChild == nullptr)
        pChild = new CNode(this);
      return pChild;
    }
    
    public:
    std::vector<CEntry*> getEntries()
    {
      std::vector<CEntry*> aTotal = mEntries;
      for(size_t i = 0; i < 2; ++i) // x
      for(size_t j = 0; j < 2; ++j) // y
      for(size_t k = 0; k < 2; ++k) // z
      {
        size_t key = i * 2 * 2 + j * 2 + k;
        CNode*& pChild = mChildren[key];
        if(pChild != nullptr) 
        {
          std::vector<CEntry*> aPart = pChild->getEntries();
          util::merge(aTotal, aPart);
        }
      }
      return aTotal;
    }
    
    void clear()
    {
      std::cout << "\t" << "CNode[" << mId << "]::clear()" << std::endl;
      
      mEntries.empty();
      for(size_t i = 0; i < 2; ++i) // x
      for(size_t j = 0; j < 2; ++j) // y
      for(size_t k = 0; k < 2; ++k) // z
      {
        size_t key = i * 2 * 2 + j * 2 + k;
        CNode*& pChild = mChildren[key];
        delete pChild;
        pChild = nullptr;
      }
      mDirty = true;
    }
    
    void update()
    {
      //std::for_each(mChildren.begin(), mChildren.end(), [](CNode*) -> void { ... });
      for(size_t i = 0; i < 2; ++i) // x
      for(size_t j = 0; j < 2; ++j) // y
      for(size_t k = 0; k < 2; ++k) // z
      {
        size_t key = i * 2 * 2 + j * 2 + k;
        CNode*& pChild = mChildren[key];
        if(pChild != nullptr)
          pChild->update();
      }
    }
    
    void resize()
    {
      // @todo
      // loop children
      // loop entries
      // collapse AABB to entries total AABB
    }
    
    void refresh()
    {
      std::cout << "\t" << "CNode[" << mId << "]::refresh()" << std::endl;
      std::vector<CEntry*> aEntries = getEntries();
      clear();
      for(size_t i = 0; i < aEntries.size(); ++i)
      {
        CEntry*& pEntry = aEntries[i];
        _insert(this, pEntry);
      }
    }
    
    void insert(CEntry* pEntry) throw(CException)
    {
      math::CBox oAABB = mAABB + pEntry->mAABB; // merge/expand AABB
      //std::cout << "\t" << mAABB << "+" << pEntry->mAABB << "=" << oAABB << std::endl;
      if(oAABB != mAABB && mAABB.getVolume() != 0.0f) // change in volume
      {
        std::cout << "\t" << "CNode[" << mId << "]::insert(" << *pEntry << ") change" << std::endl;
        mEntries.push_back(pEntry);
        mAABB = oAABB;
        refresh();
        // ? track back
      }
      else                                            // no change
      {
        std::cout << "\t" << "CNode[" << mId << "]::insert(" << *pEntry << ") no change" << std::endl;
        mAABB = oAABB;
        _insert(this, pEntry);
      }
      
      // @todo:
      // if new AABB require moving entries from children nodes
      
      // @todo:
      // mEntries.push_back(pEntry);                 // [1st] 
      // for(size_t i = 0. i < mEntries.size(); ++i) // [2nd] 
      //   CEntry*& pEntry = mEntries[i];
    }
    
    protected:
    static void _insert(CNode* pNode, CEntry* pEntry)
    {
      if(pEntry->mAABB.mPosition == pNode->mAABB.mPosition) // entry at root
      {
        std::cout << "\t" << "CNode["<< pNode->mId <<"]::insert(" << *pEntry << ") insert:EQUAL" << std::endl;
        pNode->mEntries.push_back(pEntry);
      }
      else
      {
        // @todo:
        // check pEntry position against each AABB fragment
        // if INSIDE fragment 
        //   pChild->insert(pEntry)     // insert in child
        // else if COLLIDE/INTERSECT w/ one of fragments
        //   pNode->mEntries.push_back(pEntry) // insert here
        
        std::array<math::CBox, 8> aAABBs = math::osplit(pNode->mAABB); // qslit, bsplit
        for(size_t i = 0; i < 2; ++i) // x
        for(size_t j = 0; j < 2; ++j) // y
        for(size_t k = 0; k < 2; ++k) // z
        {
          size_t key = i * 2 * 2 + j * 2 + k;
          math::CBox& oAABB = aAABBs[key];
          
          if(math::isInside(pEntry->mAABB.mPosition, oAABB)) // if position in fragment box
          {
            math::compare::result result = math::compare(pEntry->mAABB, oAABB);
            if(result == math::compare::COLLIDE)
            {
              std::cout << "\t" << "CNode["<< pNode->mId <<"]::insert(" << *pEntry << ") insert:COLLIDE" << std::endl;
              pNode->mEntries.push_back(pEntry);
            }
            else if(result == math::compare::INSIDE || result == math::compare::EQUAL)
            {
              std::cout << "\t" << "CNode["<< pNode->mId <<"]::insert(" << *pEntry << ") child[" << key << "]" << std::endl;
            
              CNode*& pChild = pNode->mChildren[key];
              if(pChild == nullptr)
                pChild = new CNode(pNode, oAABB);
              pChild->insert(pEntry);
            }
            return;
          }
        }
      }
    }
  };
  unsigned int CNode::sId = 0; 
  
  class CTree
  {
    protected:
    CNode* mRoot;
    
    public:
    CTree() { }
    
    virtual ~CTree()
    {
      delete mRoot;
      mRoot = nullptr;
    }
    
    virtual void insert(CEntry*) = 0;
  };

  class COcTree : public CTree
  {
    public:
    COcTree() : CTree()
    {
      std::cout << "COcTree::COcTree()" << std::endl;
      mRoot = new CNode(nullptr);
    }
    
    ~COcTree()
    {
      std::cout << "COcTree::~COcTree()" << std::endl;
    }

    public:
    void insert(CEntry* pEntry)
    {
      std::cout << "COcTree::insert(" << *pEntry << ")" << std::endl;
      mRoot->insert(pEntry);
    }
  };
}

#endif // __tree_coctree_hpp__
