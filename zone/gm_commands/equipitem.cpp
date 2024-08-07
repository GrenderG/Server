#include "../client.h"

void command_equipitem(Client *c, const Seperator *sep){
	uint32 slot_id = atoi(sep->arg[1]);
	if (sep->IsNumber(1) && ((slot_id >= EQ::invslot::EQUIPMENT_BEGIN) && (slot_id <= EQ::invslot::EQUIPMENT_END))) {
		const EQ::ItemInstance* from_inst = c->GetInv().GetItem(EQ::invslot::slotCursor);
		const EQ::ItemInstance* to_inst = c->GetInv().GetItem(slot_id); // added (desync issue when forcing stack to stack)
		bool partialmove = false;
		int16 movecount;

		if (from_inst && from_inst->IsClassCommon()) {
			auto outapp = new EQApplicationPacket(OP_MoveItem, sizeof(MoveItem_Struct));
			MoveItem_Struct* mi = (MoveItem_Struct*)outapp->pBuffer;
			mi->from_slot = EQ::invslot::slotCursor;
			mi->to_slot = slot_id;
			// mi->number_in_stack	= from_inst->GetCharges(); // replaced with con check for stacking

			// crude stackable check to only 'move' the difference count on client instead of entire stack when applicable
			if (to_inst && to_inst->IsStackable() &&
				(to_inst->GetItem()->ID == from_inst->GetItem()->ID) &&
				(to_inst->GetCharges() < to_inst->GetItem()->StackSize) &&
				(from_inst->GetCharges() > to_inst->GetItem()->StackSize - to_inst->GetCharges())) {
				movecount = to_inst->GetItem()->StackSize - to_inst->GetCharges();
				mi->number_in_stack = (uint32)movecount;
				partialmove = true;
			}
			else
				mi->number_in_stack = from_inst->GetCharges();

			// Save move changes
			// Added conditional check to packet send..would have sent change even on a swap failure..whoops!

			if (partialmove) { // remove this con check if someone can figure out removing charges from cursor stack issue below
				// mi->number_in_stack is always from_inst->GetCharges() when partialmove is false
				c->Message(Chat::Red, "Error: Partial stack added to existing stack exceeds allowable stacksize");
				safe_delete(outapp);
				return;
			}
			else if (c->SwapItem(mi) == 1) {
				c->FastQueuePacket(&outapp);
				return;
			}
			else {
				c->Message(Chat::Red, "Error: Unable to equip current item");
			}
			safe_delete(outapp);

			// also send out a wear change packet?
		}
		else if (from_inst == nullptr)
			c->Message(Chat::Red, "Error: There is no item on your cursor");
		else
			c->Message(Chat::Red, "Error: Item on your cursor cannot be equipped");
	}
	else
		c->Message(Chat::White, "Usage: #equipitem slotid[0-21] - equips the item on your cursor to the position");
}

