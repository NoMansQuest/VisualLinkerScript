#ifndef CDRCCOMMON_H
#define CDRCCOMMON_H

namespace VisualLinkerScript::DrcEngine::Rules
{
    class CDrcInterfaceRegistration
    {
        typedef map<string, Creator> CreatorMap;
    public:
        InterfaceRegistration& instance() {
            static InterfaceRegistration interfaceRegistration;
            return interfaceRegistration;
        }

        bool registerInterface( const string& name, Creator creator )
        {
            return (m_interfaces[name] = creator);
        }

        list<string> names() const
        {
            list<string> nameList;
            transform(
                m_interfaces.begin(), m_interfaces.end(),
                back_inserter(nameList)
                select1st<CreatorMap>::value_type>() );
        }

        Interface* create(cosnt string& name ) const
        {
            const CreatorMap::const_iterator it
                = m_interfaces.find(name);
            if( it!=m_interfaces.end() && (*it) )
            {
                return (*it)();
            }
            // throw exception ...
            return 0;
        }

    private:
        CreatorMap m_interfaces;
    };

}

#endif // CDRCCOMMON_H
