package game.entities

import scala.collection.SortedSet
import scala.collection.immutable.BitSet
import scala.collection.mutable.ArrayBuffer

/**
  * Created by haim7 on 07 ספטמבר 2016.
  */
abstract class EnumFactory {

  protected type Value <: EnumValue

  private var _values = new ArrayBuffer[Value]

  lazy val values: Seq[Value] = {
    val values = _values.toList
    _values = null
    values
  }

  def Value(name: String) = new EnumValue(name)

  protected class EnumValue (private val name: String)  {

    val id: Int = _values.size
    _values += this.asInstanceOf[Value]

    final override def equals(obj: Any): Boolean = super.equals(obj)
    final override def hashCode(): Int = id

    def +(that: Value): Set[Value] = new EnumSet(BitSet(this.id, that.id))

    override def toString: String = name
  }

  class EnumSet private[EnumFactory](bitSet: BitSet) extends Set[Value] {

    override def +(value: Value): Set[Value] = new EnumSet(bitSet + value.id)

    override def contains(value: Value): Boolean = {
      val id = value.id
      bitSet.contains(value.id) && id < size
    }

    override def -(value: Value): Set[Value] = new EnumSet(bitSet - value.id)

    override def iterator: Iterator[Value] = bitSet.iterator.take(size).map(values)

    override val size: Int = bitSet.size.min(values.size)

  }

}
object EnumFactory {
  //def valuesOf[Factory <: EnumFactory](factory: Factory) = factory.values

}
/*object SimpleEnumFactory extends EnumFactory[_] {
  def value(name: String): EnumValue =  new EnumValue(name) {}
}*/
