/*
 * Copyright (c) 2021 Valdemar Lindberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _CXXCONFIG_ITREE_H_
#define _CXXCONFIG_ITREE_H_ 1
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>

namespace cxxconfig {

	/**
	 * @brief
	 *
	 * @tparam T
	 */
	template <class T> class ITree {
	  public:
		ITree() {
			this->sibling = nullptr;
			this->numChildren = 0;
			this->child = nullptr;
			this->parent = nullptr;
		}
		ITree(ITree &&other) {
			this->parent = std::exchange(other.parent, nullptr);
			this->child = std::exchange(other.child, nullptr);
			this->sibling = std::exchange(other.sibling, nullptr);
			this->numChildren = other.numChildren;
		}
		virtual ~ITree() {}

		ITree<T> &operator=(const ITree<T> &other) { return *this; }
		ITree<T> &operator=(ITree<T> &&other) { return *this; }

		virtual ITree<T> *root() const noexcept {
			if (this->getParent() != nullptr)
				return (ITree<T> *)this;
			else
				return this->getParent()->root();
		}

		virtual bool isEmpty() const { return this->getNumChildren() == 0; }

		virtual ITree<T> *getParent() const { return this->parent; }

		virtual void setParent(ITree<T> *parent) { this->parent = parent; }

		virtual unsigned int getNumChildren() const { return this->numChildren; }

		virtual void addChild(ITree<T> *pchild) {
			ITree<T> *find;
			assert(pchild);

			this->numChildren++;
			if (!this->child) {
				this->setChild(pchild);
				child->setParent(this);
			} else {

				find = this->child;
				while (find) {
					if (find->sibling)
						find = find->sibling;
					else
						break;
				}
				find->sibling = pchild;
				find->sibling->setParent(this);
			}
		}

		virtual void removeChild(unsigned int index) {
			ITree<T> *sn = getChild(index - 1);
			ITree<T> *n = sn->sibling;
			sn->setSibling(n->sibling);
			n->parent = nullptr;
		}

		virtual ITree<T> *getChild(unsigned int index) const {
			ITree<T> *chi = this->child;
			for (unsigned int x = 0; x <= index; x++) {
				chi = chi->sibling;
			}
			return chi;
		}

		virtual int getNodeChildIndex(ITree<T> *node) {
			assert(node);
			ITree<T> *n = this->child;
			int i = 0;
			while (n) {
				/*	Compare objects.	*/
				if (n == node) {
					return i;
				}
				i++;
				n = n->child;
			}
			return -1;
		}

		/*  TODO determine if iterator can be added.    */
		// virtual std::iterator<T> begin(){
		//
		//}
		// virtual TIterator<T> end(){
		//	return I
		//}

	  protected: /*  */
		void setSibling(ITree<T> *sibling) { this->sibling = sibling; }

		void setChild(ITree<T> *child) { this->child = child; }

	  private:					  /*  */
		ITree<T> *parent;		  /*	parent node.	*/
		ITree<T> *sibling;		  /*	sibling node.	*/
		ITree<T> *child;		  /*	child node.	*/
		unsigned int numChildren; /*	number of children node attached.	*/
	};
} // namespace cxxconfig
#endif
