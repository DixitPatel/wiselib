
#include "external_interface/external_interface.h"
#include "algorithms/routing/tree/tree_routing.h"

typedef wiselib::OSMODEL Os;
class MyApplication{

private:
         Os::Radio::self_pointer_t radio_;
         Os::Debug::self_pointer_t debug_;
         Os::Timer::self_pointer_t timer_; 
public:
	void init(Os::AppMainParameter& param_value){
       
//getting Os facets
	radio_ = &wiselib::FacetProvider<Os, Os::Radio>::get_facet(param_value);	
        timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet(param_value);	 
	debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet(param_value);	

//print message
        debug_->debug("Hi, this is my first application");

      	radio_->reg_recv_callback<MyApplication, &MyApplication::receive_radio_message>(this);     
	timer_->set_timer<MyApplication, &MyApplication::start>(3000, this, 0 );
}

//---------------------------------------------------------------------------------------------------
	void start(void*){	
        debug_->debug("broadcast from %d \n", radio_->id());
	Os::Radio::block_data_t message[]="hi peeps!\0";             
	radio_->send(Os::Radio::BROADCAST_ADDRESS, sizeof(message), message);
}

//---------------------------------------------------------------------------------------------------

	void receive_radio_message(Os::Radio::node_id_t sender, Os::Radio::size_t length, Os::Radio::block_data_t *buf)
  	{
	 debug_->debug("received message to node %u from node %u \n", radio_->id(), sender);
     	debug_->debug("received message is %s", buf);
	}
//----------------------------------------------------------------------------------------------------

};


wiselib::WiselibApplication<Os, MyApplication> my_app;
	void application_main(Os::AppMainParameter& param_value){
 	my_app.init(param_value);
      }	
