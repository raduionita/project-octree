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
    unsigned int           mId;
    CNode*                 mParent;
    std::array<CNode*, 8u> mChildren; // 8 children
    std::vector<CEntry*>   mEntries;  // data
    math::CBox             mAABB;
    bool                   mDirty;
    
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

    public:
    /**
     * Get child node by index
     * @param  ushort i
     * @return CNode*   child ndoe
     */ 
    CNode* getNode(ushort i)
    {
      assert(i < 8);
      CNode*& pChild = mChildren[i];
      if(pChild == nullptr)
        pChild = new CNode(this);
      return pChild;
    }
    
    /**
     * Find node(child or this) by position inside this node
     * @param  vec3&  vPosition
     * @return CNode*           this or child
     */
    CNode* getNode(const math::vec3& vPosition)
    {
      // get node by position
      if(vPosition == mAABB.mPosition)
        return this;
      else if(vPosition.x < mAABB.mPosition.x - mAABB.mWidth/2.0f  || mAABB.mPosition.x + mAABB.mWidth/2.0f  < vPosition.x
           || vPosition.y < mAABB.mPosition.y - mAABB.mHeight/2.0f || mAABB.mPosition.y + mAABB.mHeight/2.0f < vPosition.y
           || vPosition.z < mAABB.mPosition.z - mAABB.mDepth/2.0f  || mAABB.mPosition.z + mAABB.mDepth/2.0f  < vPosition.z)
        return nullptr;
      //ushort i = (vPosition.x > mAABB.mPosition.x) ? 1 : 0, 
      //       j = (vPosition.y > mAABB.mPosition.y) ? 1 : 0, 
      //       k = (vPosition.z > mAABB.mPosition.z) ? 1 : 0;
      return mChildren[(ushort)(vPosition.x > mAABB.mPosition.x) * 4 + // i * 2 * 2 +
                       (ushort)(vPosition.y > mAABB.mPosition.y) * 2 + // j * 2 +
                       (ushort)(vPosition.z > mAABB.mPosition.z)];     // k
    }
    
    CNode* getNodeRecursive(const math::vec3& vPosition)
    {
      // @todo
      // recursive version of getNode(vec3)
      return nullptr;
    }
    
    std::vector<CEntry*> getEntries(bool bRecursive = false)
    {
      std::vector<CEntry*> aTotal = mEntries;
      if(bRecursive)
      {
        for(size_t i = 0; i < 2; ++i) // x
        for(size_t j = 0; j < 2; ++j) // y
        for(size_t k = 0; k < 2; ++k) // z
        {
          size_t key = i * 2 * 2 + j * 2 + k;
          CNode*& pChild = mChildren[key];
          if(pChild != nullptr) 
          {
            std::vector<CEntry*> aPart = pChild->getEntries(true);
            util::merge(aTotal, aPart);
          }
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
      
      // @todo
      // check all entries to see if there're still in this node
      // an entry might have moved or changed its volume
      for(size_t i = 0; i < mEntries.size(); ++i)
      {
        CEntry* pEntry = mEntries[i];
        if(pEntry != nullptr)
        {
          // if pEntry insde this(node)
          // && compare pEntry, this(node) == INSIDE
          //   leave it here
          // else
          //   remove/pop pEntry from this(node)
          //   mParent->insert(pEntry)
          //
        }
      }
      
      
      // @todo
      // maybe the volume of the node has changed, and(probably) the volume of the parent
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
      _refresh(this);
    }
    
    void insert(CEntry* pEntry) throw(CException)
    {
      math::CBox oAABB = mAABB + pEntry->mAABB; // merge/expand AABB
      if(oAABB != mAABB && mAABB.getVolume() != 0.0f) // change in volume && not empty
      {
        std::cout << "\t" << "CNode[" << mId << "]::insert(" << *pEntry << ") change" << std::endl;
        mEntries.push_back(pEntry);
        mAABB = oAABB;
        _refresh(this);
      }
      else                                            // no change || empty
      {
        std::cout << "\t" << "CNode[" << mId << "]::insert(" << *pEntry << ") no change" << std::endl;
        mAABB = oAABB;
        _insert(this, pEntry);
      }
      
      // @todo:
      // if new AABB require moving entries from children nodes
    }
    
    protected:
    static void _refresh(CNode* pNode) throw(CException)
    {
      std::cout << "\t" << "CNode[" << pNode->mId << "]::refresh()" << std::endl;
      std::vector<CEntry*> aEntries = pNode->getEntries(true);
      pNode->clear();
      for(size_t i = 0; i < aEntries.size(); ++i)
      {
        CEntry*& pEntry = aEntries[i];
        _insert(pNode, pEntry);
      }
    }
    
    static void _insert(CNode* pNode, CEntry* pEntry) throw(CException)
    {
      // @todo:
      // get destination node by pEntry position
      // @see getNode(vec3)
      // if node == this/pNode
      //   insert here/pNode/parent/this
      // else
      //   insert in node
    
      if(pEntry->mAABB.mPosition == pNode->mAABB.mPosition) // entry at root
      {
        std::cout << "\t" << "CNode["<< pNode->mId <<"]::insert(" << *pEntry << ") insert:EQUAL" << std::endl;
        pNode->mEntries.push_back(pEntry);
      }
      else
      {
        std::array<math::CBox, 8> aAABBs = math::osplit(pNode->mAABB); // qslit, bsplit
        for(size_t i = 0; i < 2; ++i) // x
        for(size_t j = 0; j < 2; ++j) // y
        for(size_t k = 0; k < 2; ++k) // z
        {
          size_t key = i * 2 * 2 + j * 2 + k;
          math::CBox& oAABB = aAABBs[key];
          
          if(math::inside(pEntry->mAABB.mPosition, oAABB)) // if position in fragment box
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
    CTree() : mRoot(nullptr) 
    { 
      
    }
    
    virtual ~CTree()
    {
      delete mRoot;
      mRoot = nullptr;
    }
    
    virtual void insert(CEntry*) = 0;
    
    CNode* getRoot() const
    {
      return mRoot;
    }
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
  
    void update()
    {
      mRoot->update();
    }
  };

  class CResult
  {
    public:
    void clear()
    {
    
    }
  };
  
  class CQuery
  {
    public:
    enum EField
    {
      NODE,
      ENTRY
    };
    
    enum EComp
    {
      IN,
      LESSER,
      LEQUAL,
      EQUAL,
      GEQUAL,
      GREATER
    };

    enum EOrder
    {
      ASC,
      DESC
    };
    
    protected:
    CResult mResult;
    
    public:
    CQuery()
    {
      
    }
    
    virtual ~CQuery()
    {
      
    }
    
    public:
    CQuery& select(EField field)
    {
      return *this;
    }
    
    CQuery& from(const COcTree& oOcTree)
    {
      return from(oOcTree.getRoot());
    }
    
    CQuery& from(const CNode& pNode)
    {
      return *this;
    }
    
    CQuery& where(EField field, EComp comp, const math::CFrustum& frustum)
    {
      return *this;
    }
    
    CQuery& order()
    {
      return *this;
    }
    
    CResult& execute()
    {
      mResult.clear();
      
      return mResult;
    }
  };
}

#endif // __tree_coctree_hpp__
