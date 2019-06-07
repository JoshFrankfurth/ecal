/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

/**
 * @file   publisher.h
 * @brief  eCAL publisher interface for Cap'n Proto message definitions
**/

#pragma once

#include <ecal/msg/publisher.h>

// capnp includes
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100 4239 4244 4245)
#endif
#include <capnp/serialize.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif


namespace eCAL
{
  namespace capnproto
  {
    /**
     * @brief eCAL capnp publisher class.
     *
     * Publisher template  class for capnp messages. For details see documentation of CPublisher class.
     *
    **/
    template <typename T>
    class CPublisher : public CMsgPublisher<T>
    {
    public:
      /**
       * @brief  Constructor.
      **/
      CPublisher() : CMsgPublisher<T>()
      {
      }

      /**
       * @brief  Constructor.
       *
       * @param topic_name_  Unique topic name.
      **/
      CPublisher(const std::string& topic_name_) : CMsgPublisher<T>(topic_name_, GetTypeName(), GetDescription())
      {
      }

      /**
      * @brief  Copy Constructor is not available.
      **/
      CPublisher(const CPublisher&) = delete;

      /**
      * @brief  Copy Constructor is not available.
      **/
      CPublisher& operator=(const CPublisher&) = delete;

      /**
      * @brief  Move Constructor
      **/
      CPublisher(CPublisher&&) = default;

      /**
      * @brief  Move assignment
      **/
      CPublisher& operator=(CPublisher&&) = default;

      /**
       * @brief  Creates this object.
       *
       * @param topic_name_  Unique topic name.
       *
       * @return  True if it succeeds, false if it fails.
      **/
      bool Create(const std::string& topic_name_)
      {
        return(CMsgPublisher<T>::Create(topic_name_, GetTypeName(), GetDescription()));
      }

      /**
       * @brief  Get type name of the capnp message.
       *
       * @return  Type name.
      **/
      std::string GetTypeName() const
      {
        return("capnp:");
      }

    private:
      /**
       * @brief  Get file descriptor string of the capnp message.
       *
       * @return  Description string.
      **/
      std::string GetDescription() const
      {
        return("");
      }

      /**
       * @brief  Get size for serialized message object.
       *
       * @param msg_  The message object.
       *
       * @return  Message size.
      **/
      size_t GetSize(const T& msg_) const
      {
        return(capnp::computeSerializedSizeInWords(const_cast<T&>(msg_)) * sizeof(capnp::word));
      }

      /**
       * @brief  Serialize the message object into a preallocated char buffer.
       *
       * @param       msg_     The message object.
       * @param [out] buffer_  Target buffer.
       * @param       size_    Target buffer size.
       *
       * @return  True if it succeeds, false if it fails.
      **/
      bool Serialize(const T& msg_, char* buffer_, size_t size_) const
      {
        kj::Array<capnp::word> words = capnp::messageToFlatArray(const_cast<T&>(msg_));
        kj::ArrayPtr<kj::byte> bytes = words.asBytes();
        if (size_ < bytes.size()) return(false);
        memcpy(buffer_, bytes.begin(), bytes.size());
        return(true);
      }
    };
    /** @example addressbook_snd.cpp
    * This is an example how to use eCAL::CPublisher to send capnp data with eCAL. To receive the data, see @ref addressbook_rec.cpp .
    */
  }
}