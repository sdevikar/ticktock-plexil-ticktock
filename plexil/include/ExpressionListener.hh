/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PLEXIL_EXPRESSION_LISTENER_HH
#define PLEXIL_EXPRESSION_LISTENER_HH

namespace PLEXIL
{
  //
  // Forward declarations
  //
  class Expression;

  /**
   * @brief Abstract base class for listeners in the expression notification graph.
   * An expression listener may listen to multiple expressions.
   * A listener may be owned by another expression, a node, or some other object.
   */
  class ExpressionListener
  {
  public:

    ExpressionListener() {}
    ExpressionListener(ExpressionListener const &) {}
    ExpressionListener &operator=(ExpressionListener const &) 
    { return *this; }

    virtual ~ExpressionListener() {}

    /**
     * @brief Virtual function for notification that an expression's value has changed.
     * @param src The source of the notification, so that recipients can check for circularity.
     *            (e.g. an array reference modifying its array)
     */
    virtual void notifyChanged(Expression const *src) = 0;
  };

}

#endif // PLEXIL_EXPRESSION_LISTENER_HH
